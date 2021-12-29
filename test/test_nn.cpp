#include <QObject>
#include <QTest>

#include "neuralnetwork/nn.h"
#include <iostream>

int random_between(int min, int max) {
  std::random_device
      rd; // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<> distrib(min, max);

  return distrib(gen);
}

class testNN : public QObject {

  Q_OBJECT

private slots:

  void test_xor_ai() {

    struct TrainingData {
      std::vector<double> inputs;
      std::vector<double> outputs;
    };

    std::vector<TrainingData> training_data = {{{0, 0}, {0}},
                                               {{1, 0}, {1}},
                                               {{0, 1}, {1}},
                                               {{1, 1}, {0}}}

    ;

    NeuralNetwork nn(2, 4, 1);
    nn.setLearningRate(0.1);

    for (int i = 0; i < 50000; i++) {
      int index = random_between(0, 3);
      auto data = training_data[index];
      nn.train(data.inputs, data.outputs);
      // std::cout << index;
    }

    for (auto data : training_data) {
      auto output = nn.predict(data.inputs);

      std::cout << data.inputs[0] << " xor " << data.inputs[1] << " = "
                << output[0] << " expected " << data.outputs[0]
                << " error= " << data.outputs[0] - output[0] << '\n';

      QVERIFY(std::fabs(data.outputs[0] - output[0]) < 0.1);
    }
  }
};
QTEST_MAIN(testNN)
#include "test_nn.moc"