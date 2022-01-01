#pragma once
#include <functional>
#include <random>
#include <vector>

struct Matrix {

  int rows;
  int cols;
  std::vector<std::vector<double>> data;
  static std::random_device RandomDevice;

  Matrix(int rows, int cols)
      : rows{rows}, cols{cols}, data(rows, std::vector<double>(cols, 0.0)) {}

  static Matrix fromArray(const std::vector<double> &array) {
    Matrix m{1, static_cast<int>(array.size())};
    m.data[0] = array;

    return Matrix::transpose(m);
  }

  static Matrix subtract(const Matrix &a, const Matrix &b) {
    if (a.rows != b.rows || a.cols != b.cols) {
      throw std::runtime_error("Matrix::substract  ; Columns and Rows of A "
                               "must match Columns and Rows of B.");
    }

    // Return a new Matrix a-b
    return Matrix(a.rows, a.cols).map([&a, &b](double, int i, int j) {
      return a.data[i][j] - b.data[i][j];
    });
  }

  Matrix &add(const Matrix &n) {
    if (rows != n.rows || cols != n.cols) {
      throw std::runtime_error("Matrix::add  ; Columns and Rows of A must "
                               "match Columns and Rows of B.");
    }
    return map([&n](double val, int i, int j) { return val + n.data[i][j]; });
  }

  Matrix &add(double n) {
    return this->map([n](double val, int i, int j) { return val + n; });
  }

  void forEach(std::function<void(double &, int, int)> cb) {
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        cb(data[i][j], i, j);
      }
    }
  }

  void forEach(std::function<void(double, int, int)> cb) const {
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        double val = data[i][j];
        cb(val, i, j);
      }
    }
  }

  void forEach(std::function<void(double &)> cb) {
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        cb(data[i][j]);
      }
    }
  }

  void forEach(std::function<void(double)> cb) const {
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        cb(data[i][j]);
      }
    }
  }

  double &at(int rows, int cols) { return data.at(rows).at(cols); }

  double at(int rows, int cols) const { return data.at(rows).at(cols); }

  std::vector<double> toArray() const {
    std::vector<double> arr;
    arr.reserve(cols * rows);

    forEach([&arr](double val, int, int) { arr.push_back(val); });

    return arr;
  }

  Matrix &randomize() {

    std::mt19937 gen(RandomDevice());
    std::uniform_real_distribution<> distrib(0.0, 1.0);

    return map([&distrib, &gen](double, int, int) { return distrib(gen); });
  }

  static Matrix transpose(const Matrix &matrix) {
    return Matrix(matrix.cols, matrix.rows)
        .map([&matrix](double, int i, int j) { return matrix.data[j][i]; });
  }

  static Matrix multiply(const Matrix &a, const Matrix &b) {
    // Matrix product
    if (a.cols != b.rows) {
      throw std::runtime_error(
          "Matrix::multiply; Columns of A must match rows of B.");
    }

    return Matrix(a.rows, b.cols).map([&a, &b](double e, int i, int j) {
      // Dot product of values in col
      double sum = 0;
      for (int k = 0; k < a.cols; k++) {
        sum += a.data[i][k] * b.data[k][j];
      }
      return sum;
    });
  }

  Matrix &multiply(const Matrix &n) {
    if (rows != n.rows || cols != n.cols) {
      throw std::runtime_error("Matrix::multiply  ; Columns and Rows of A must "
                               "match Columns and Rows of B.");
    }
    // hadamard product
    return this->map(
        [&n](double val, int i, int j) { return val * n.data[i][j]; });
  }

  Matrix &multiply(double n) {
    // Scalar product
    return this->map([n](double val, int i, int j) { return val * n; });
  }

  Matrix &map(std::function<double(double, int, int)> cb) {
    // Apply a function to every element of matrix

    forEach([cb](double &val, int row, int col) { val = cb(val, row, col); });

    return *this;
  }

  Matrix &map(std::function<double(double)> cb) {
    // Apply a function to every element of matrix

    forEach([cb](double &val) { val = cb(val); });

    return *this;
  }

  static Matrix map(const Matrix &matrix,
                    std::function<double(double, int, int)> cb) {
    // Apply a function to every element of matrix
    return Matrix(matrix.rows, matrix.cols)
        .map([&cb, &matrix](double, int i, int j) {
          return cb(matrix.data[i][j], i, j);
        });
  }

  static Matrix map(const Matrix &matrix, std::function<double(double)> cb) {
    // Apply a function to every element of matrix
    // Apply a function to every element of matrix
    return Matrix(matrix.rows, matrix.cols)
        .map([&cb, &matrix](double, int i, int j) {
          return cb(matrix.data[i][j]);
        });
  }

  std::string serialize() const {
    throw std::runtime_error("Matrix::serialise TODO");
  }

  static Matrix deserialize(const std::string &data) {
    throw std::runtime_error("Matrix::serialise TODO");
    return Matrix{0, 0};
  }

  bool operator==(const Matrix &b) const {
    if (rows != b.rows || cols != b.cols) {
      return false;
    }
    return data == b.data;
  }
};

#include <iomanip> // std::setw
static std::ostream &operator<<(std::ostream &out, const Matrix &m) {
  int newline = 0;

  m.forEach([&out, &newline](double val, int row, int col) {
    if (newline != row) {
      out << '\n';
      newline++;
    }
    out << std::setw(10) << val;
  });
  return out;
}

std::random_device Matrix::RandomDevice{};
