#include <QObject>
#include <QTest>

#include "neuralnetwork/matrix.h"
#include "neuralnetwork/nn.h"
#include <iostream>
class testMatrix : public QObject {

  Q_OBJECT

private slots:

  void can_create_matrix() {
    Matrix m{3, 4};
    m.randomize();
    std::cout << m << std::endl;
  }

  void add_scalar_to_matrix() {
    Matrix m(3, 3);
    m.data[0] = {1.0, 2.0, 3.0};
    m.data[1] = {4.0, 5.0, 6.0};
    m.data[2] = {7.0, 8.0, 9.0};
    m.add(1);

    Matrix exp(3, 3);
    exp.data[0] = {2.0, 3.0, 4.0};
    exp.data[1] = {5.0, 6.0, 7.0};
    exp.data[2] = {8.0, 9.0, 10.0};

    QVERIFY(m == exp);
  }

  void add_matrix_to_other_matrix() {
    Matrix m(2, 2);
    m.data[0] = {1, 2};
    m.data[1] = {3, 4};
    Matrix n(2, 2);
    n.data[0] = {10, 11};
    n.data[1] = {12, 13};
    m.add(n);

    Matrix exp(2, 2);
    exp.data[0] = {11.0, 13.0};
    exp.data[1] = {15.0, 17.0};

    QVERIFY(m == exp);
  }

  void subtract_matrix_from_other_matrix() {
    Matrix m(2, 2);
    m.data[0] = {1, 2};
    m.data[1] = {3, 4};
    Matrix n(2, 2);
    n.data[0] = {10, 11};
    n.data[1] = {12, 13};

    Matrix l = Matrix::subtract(n, m);

    Matrix exp(2, 2);
    exp.data[0] = {9.0, 9.0};
    exp.data[1] = {9.0, 9.0};

    QVERIFY(l == exp);
  }

  void matrix_product() {
    Matrix m(2, 3);
    m.data[0] = {1, 2, 3};
    m.data[1] = {4, 5, 6};
    Matrix n(3, 2);
    n.data[0] = {7, 8};
    n.data[1] = {9, 10};
    n.data[2] = {11, 12};

    Matrix l = Matrix::multiply(m, n);

    Matrix exp(2, 2);
    exp.data[0] = {58, 64};
    exp.data[1] = {139, 154};

    QVERIFY(l == exp);
  }

  void hadamard_product() {
    Matrix m(3, 2);
    m.data[0] = {1, 2};
    m.data[1] = {3, 4};
    m.data[2] = {5, 6};
    Matrix n(3, 2);
    n.data[0] = {7, 8};
    n.data[1] = {9, 10};
    n.data[2] = {11, 12};

    m.multiply(n);

    Matrix exp(3, 2);
    exp.data = {{7, 16}, {27, 40}, {55, 72}};

    QVERIFY(m == exp);
  }

  void scalar_product() {
    Matrix m(3, 2);
    m.data[0] = {1, 2};
    m.data[1] = {3, 4};
    m.data[2] = {5, 6};

    m.multiply(7);

    Matrix exp(3, 2);
    exp.data = {{7, 14}, {21, 28}, {35, 42}};

    QVERIFY(m == exp);
  }
  void transpose_matrix_1_1() {
    Matrix m(1, 1);
    m.data[0] = {1};
    auto mt = Matrix::transpose(m);

    QVERIFY(m == mt);
  }

  void transpose_matrix_2_3() {
    Matrix m(2, 3);
    m.data = {{1, 2, 3}, {4, 5, 6}};
    auto mt = Matrix::transpose(m);

    Matrix exp(3, 2);
    exp.data = {{1, 4}, {2, 5}, {3, 6}};

    QVERIFY(exp == mt);
  }

  void transpose_matrix_3_2() {
    Matrix m(3, 2);
    m.data[0] = {1, 2};
    m.data[1] = {3, 4};
    m.data[2] = {5, 6};
    auto mt = Matrix::transpose(m);

    Matrix exp(2, 3);
    exp.data = {{1, 3, 5}, {2, 4, 6}};

    QVERIFY(exp == mt);
  }

  void mapping_with_static_map() {
    Matrix m(3, 3);
    m.data[0] = {1, 2, 3};
    m.data[1] = {4, 5, 6};
    m.data[2] = {7, 8, 9};
    auto mapped = Matrix::map(m, [](double val, int, int) { return val * 10; });

    Matrix exp(3, 3);
    exp.data[0] = {10, 20, 30};
    exp.data[1] = {40, 50, 60};
    exp.data[2] = {70, 80, 90};

    QVERIFY(exp == mapped);
  }

  void mapping_with_method_map() {
    Matrix m(3, 3);
    m.data[0] = {1, 2, 3};
    m.data[1] = {4, 5, 6};
    m.data[2] = {7, 8, 9};
    m.map([](double val, int, int) { return val * 10; });

    Matrix exp(3, 3);
    exp.data[0] = {10, 20, 30};
    exp.data[1] = {40, 50, 60};
    exp.data[2] = {70, 80, 90};

    QVERIFY(exp == m);
  }

  void
  error_handling_of_addition_when_columns_and_rows_of_A_do_not_match_columns_and_rows_of_B() {
    // Replace console.log with a jest mock so we can see if it has been called

    Matrix m1(1, 2);
    Matrix m2(3, 4);

    QVERIFY_EXCEPTION_THROWN(m1.add(m2), std::runtime_error);
  }

  void
  error_handling_of_static_substraction_when_columns_and_rows_of_A_do_not_match_columns_and_rows_of_B() {
    // Replace console.log with a jest mock so we can see if it has been called

    Matrix m1(1, 2);
    Matrix m2(3, 4);

    QVERIFY_EXCEPTION_THROWN(Matrix::subtract(m1, m2), std::runtime_error);
  }

  void
  error_handling_of_hadamard_product_when_columns_and_rows_of_A_do_not_match_columns_and_rows_of_B() {
    // Replace console.log with a jest mock so we can see if it has been called

    Matrix m1(1, 2);
    Matrix m2(3, 4);

    QVERIFY_EXCEPTION_THROWN(m1.multiply(m2), std::runtime_error);
  }

  void
  error_handling_of_matrix_product_when_columns_of_A_do_not_match_rows_of_B() {
    // Replace console.log with a jest mock so we can see if it has been called

    Matrix m1(1, 2);
    Matrix m2(3, 4);

    QVERIFY_EXCEPTION_THROWN(Matrix::multiply(m1, m2), std::runtime_error);
  }

  void matrix_from_array() {
    std::vector<double> array{1, 2, 3};
    auto m = Matrix::fromArray(array);

    Matrix exp(3, 1);
    exp.data = {{1}, {2}, {3}};
    QVERIFY(m == exp);
  }

  void matrix_to_array() {
    Matrix m(3, 3);
    m.data = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

    auto array = m.toArray();
    auto exp = std::vector<double>{1, 2, 3, 4, 5, 6, 7, 8, 9};
    QVERIFY(array == exp);
  }

  void chaining_matrix_methods() {
    Matrix m(3, 3);
    m.data = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

    m.map([](double e, int, int) { return e - 1; }).multiply(10).add(6);

    Matrix exp(3, 3);
    exp.data = {{6, 16, 26}, {36, 46, 56}, {66, 76, 86}};
  }

  void method_map_with_row_and_column_params() {
    Matrix m(3, 3);
    m.data = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

    m.map([](double e, int row, int col) { return e * 100 + row * 10 + col; });

    Matrix exp(3, 3);
    exp.data = {{100, 201, 302}, {410, 511, 612}, {720, 821, 922}};

    QVERIFY(m == exp);
  }
  void static_map_with_row_and_column_params() {
    Matrix m(3, 3);
    m.data = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

    auto mapped = Matrix::map(
        m, [](double e, int row, int col) { return e * 100 + row * 10 + col; });

    Matrix exp(3, 3);
    exp.data = {{100, 201, 302}, {410, 511, 612}, {720, 821, 922}};

    QVERIFY(mapped == exp);
  }

  void matrix_copy() {
    Matrix m(5, 5);
    m.randomize();

    auto n = m;

    QVERIFY(n == m);
  }
};
QTEST_MAIN(testMatrix)
#include "test_matrix.moc"

/*

test('matrix (de)serialization', () => {
  let m = new Matrix(5, 5);
  m.randomize();

  let n = Matrix.deserialize(m.serialize());

  expect(n).toEqual({
    rows: m.rows,
    cols: m.cols,
    data: m.data
  });
});



*/
