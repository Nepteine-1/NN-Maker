#ifndef NEURALNETWORK_HPP
#define NEURALNETWORK_HPP

#include <NeuralNetwork/Matrix.hpp>
#include <vector>
#include <math.h>
#include <cstdlib>
#include <random>
#include <iomanip>
#include <sstream>
#include <fstream>

enum Activation {SIGMOID, RELU, ELU};
enum LossFunction {BINARY_CROSS_ENTROPY};

        // GENERATE DATA
void generateData_Linear(Matrix& X_feature,Matrix& Y_class);
void generateData_Circle(Matrix& X_feature,Matrix& Y_class);
void generateData_Balanced(Matrix& X_feature,Matrix& Y_class);
void generateData_3Class(Matrix& X_feature,Matrix& Y_class);

void saveData(const Matrix& train_X, const Matrix& train_Y, const Matrix& test_X, const Matrix& test_Y, std::string outputFileName="data.txt");
void loadData( Matrix& train_X, Matrix& train_Y, Matrix& test_X, Matrix& test_Y, std::string inputFileName="data.txt");

class NeuralNetwork {
    public:
        NeuralNetwork(const int number_of_features, const LossFunction loss_type);
        ~NeuralNetwork();

        void addLayer(const int nb_neuron, const Activation& activation_type = Activation::SIGMOID);
        void clear(void);
        void train(const Matrix& X_train, const Matrix& Y_train, const int epoch=1, const float learning_rate=1.0f, const bool show_result=true);
        void setNbFeatures(int nb_features);
        void setLossFunction(LossFunction type);
        void setLossResultsEach(int value);
        Matrix& getLossResult(void);
        std::vector<int>& getEpochResult(void);
        std::vector<float>& getAccuResult(void);
        int getTrainPercent(void);
        Matrix predict(const Matrix& X_test, const Matrix& Y_test);

        float calculateAccu(Matrix const& Y, Matrix const& Y_train);

    private:
        // CHANGE ACTIVATION AND LOSSFUNCTION DURING LOSS CALCULATION AND BACKPROPAGATION
        void setActivationFunction(Activation type);
        void initWeights(const Activation& activation_type, int nb_lines, int nb_columns);
        void initBias(const Activation& activation_type, int nb_lines, int nb_columns);

        // LOSS FUNCTION AND DERIVATE
        Matrix LogLoss(const Matrix& A, const Matrix& Y);
        Matrix LogLossDerivate(const Matrix& A, const Matrix& Y);

        // ACTIVATION FUNCTION AND DERIVATE
        Matrix Sigmoid(const Matrix& Z);
        Matrix SigmoidDerivate(const Matrix& Z);

        Matrix ELU(const Matrix& Z);
        Matrix ELUDerivate(const Matrix& Z);

        Matrix RELU(const Matrix& Z);
        Matrix RELUDerivate(const Matrix& Z);

    private:
        std::vector<Matrix> m_weights;
        std::vector<Matrix> m_bias;
        std::vector<Activation> m_activ_func;
        int nb_features;
        Matrix m_lossEvo;
        std::vector<int> m_epochEvo;
        std::vector<float> m_accuEvo;
        int m_getLossResultsEach = 10;
        int m_trainPercent=0;

        // Functions called when calculating activation or loss
        Matrix (NeuralNetwork::* activation) (const Matrix&);
        Matrix (NeuralNetwork::* activationDerivate) (const Matrix&);
        Matrix (NeuralNetwork::* Loss) (const Matrix&, const Matrix&);
        Matrix (NeuralNetwork::* LossDerivate) (const Matrix&, const Matrix&);
};



#endif
