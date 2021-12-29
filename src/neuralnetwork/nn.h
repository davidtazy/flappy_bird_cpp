#pragma once
#include <cmath>
#include <functional>

#include "matrix.h"
// Other techniques for learning

using AFunction = std::function<double(double)>;

struct ActivationFunction {
  AFunction func;
  AFunction dfunc;
  ActivationFunction(AFunction func, AFunction dfunc = nullptr)
      : func(func), dfunc(dfunc) {}
};

class NeuralNetwork {

  int input_nodes;
  int hidden_nodes;
  int output_nodes;
  Matrix weights_ih;
  Matrix weights_ho;
  Matrix bias_h;
  Matrix bias_o;

  double learning_rate{0.1};
  ActivationFunction activation_function{sigmoid};

public:
  // copiable
  NeuralNetwork(const NeuralNetwork &other) = default;
  NeuralNetwork &operator=(const NeuralNetwork &other) = default;

  // movable
  NeuralNetwork(NeuralNetwork &&other) = default;
  NeuralNetwork &operator=(NeuralNetwork &&other) = default;

  // clang-format off
  NeuralNetwork(int inputs, int hiddens, int outputs)
      : input_nodes{inputs}
      , hidden_nodes{hiddens}
      , output_nodes{outputs}
      , weights_ih{hidden_nodes, input_nodes}
      , weights_ho{output_nodes, hidden_nodes}
      , bias_h{hidden_nodes, 1}
      , bias_o{output_nodes, 1}
  {
    weights_ih.randomize();
    weights_ho.randomize();
    bias_h.randomize();
    bias_o.randomize();
  }
  // clang-format on

  std::vector<double> predict(const std::vector<double> &input_array) const {

    // Generating the Hidden Outputs
    auto inputs = Matrix::fromArray(input_array);
    auto hidden = Matrix::multiply(weights_ih, inputs);
    hidden.add(this->bias_h);
    // activation function!
    hidden.map(this->activation_function.func);

    // Generating the output's output!
    auto output = Matrix::multiply(this->weights_ho, hidden);
    output.add(this->bias_o);
    output.map(this->activation_function.func);

    // Sending back to the caller!
    return output.toArray();
  }

  void setLearningRate(double learning_rate) {
    this->learning_rate = learning_rate;
  }

  void setActivationFunction(ActivationFunction func) {
    this->activation_function = func;
  }

  void train(const std::vector<double> &input_array,
             const std::vector<double> &target_array) {
    // Generating the Hidden Outputs
    auto inputs = Matrix::fromArray(input_array);
    auto hidden = Matrix::multiply(this->weights_ih, inputs);
    hidden.add(this->bias_h);
    // activation function!
    hidden.map(this->activation_function.func);

    // Generating the output's output!
    auto outputs = Matrix::multiply(this->weights_ho, hidden);
    outputs.add(this->bias_o);
    outputs.map(this->activation_function.func);

    // Convert array to matrix object
    auto targets = Matrix::fromArray(target_array);

    // Calculate the error
    // ERROR = TARGETS - OUTPUTS
    auto output_errors = Matrix::subtract(targets, outputs);

    // auto gradient = outputs * (1 - outputs);
    // Calculate gradient
    auto gradients = Matrix::map(outputs, this->activation_function.dfunc);
    gradients.multiply(output_errors);
    gradients.multiply(this->learning_rate);

    // Calculate deltas
    auto hidden_T = Matrix::transpose(hidden);
    auto weight_ho_deltas = Matrix::multiply(gradients, hidden_T);

    // Adjust the weights by deltas
    this->weights_ho.add(weight_ho_deltas);
    // Adjust the bias by its deltas (which is just the gradients)
    this->bias_o.add(gradients);

    // Calculate the hidden layer errors
    auto who_t = Matrix::transpose(this->weights_ho);
    auto hidden_errors = Matrix::multiply(who_t, output_errors);

    // Calculate hidden gradient
    auto hidden_gradient = Matrix::map(hidden, this->activation_function.dfunc);
    hidden_gradient.multiply(hidden_errors);
    hidden_gradient.multiply(this->learning_rate);

    // Calcuate input->hidden deltas
    auto inputs_T = Matrix::transpose(inputs);
    auto weight_ih_deltas = Matrix::multiply(hidden_gradient, inputs_T);

    this->weights_ih.add(weight_ih_deltas);
    // Adjust the bias by its deltas (which is just the gradients)
    this->bias_h.add(hidden_gradient);

    // outputs.print();
    // targets.print();
    // error.print();
  }

  // serialize() { return JSON.stringify(this); }

  /*static deserialize(data) {
    if (typeof data == 'string') {
      data = JSON.parse(data);
    }
    let nn = new NeuralNetwork(data.input_nodes, data.hidden_nodes,
                               data.output_nodes);
    nn.weights_ih = Matrix::deserialize(data.weights_ih);
    nn.weights_ho = Matrix::deserialize(data.weights_ho);
    nn.bias_h = Matrix::deserialize(data.bias_h);
    nn.bias_o = Matrix::deserialize(data.bias_o);
    nn.learning_rate = data.learning_rate;
    return nn;
  }*/

  void mutate(double rate) {

    auto mutate = [rate](double val) {
      auto randomGaussian = [](double mean, double sd) -> double {
        std::random_device rd{};
        std::mt19937 gen{rd()};
        std::normal_distribution<> d{mean, sd};

        return d(gen);
      };

      if (Matrix{1, 1}.randomize().at(0, 0) < rate) {
        return val + randomGaussian(0, 0.1);
      } else {
        return val;
      }
    };
    this->weights_ih.map(mutate);
    this->weights_ho.map(mutate);
    this->bias_h.map(mutate);
    this->bias_o.map(mutate);
  }

  static const ActivationFunction sigmoid;

  static const ActivationFunction tanh;
};

const ActivationFunction
    NeuralNetwork::sigmoid([](double x) { return 1.0 / (1.0 + std::exp(-x)); },
                           [](double y) { return y * (1.0 - y); });

const ActivationFunction
    NeuralNetwork::tanh([](double x) { return std::tanh(x); },
                        [](double y) { return 1.0 - (y * y); });