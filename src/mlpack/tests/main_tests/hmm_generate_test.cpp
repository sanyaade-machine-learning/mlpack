/**
 * @file hmm_generate_test.cpp
 * @author Daivik Nema
 *
 * Test mlpackMain() of hmm_generate_main.cpp
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#include <string>

#define BINDING_TYPE BINDING_TYPE_TEST
static const std::string testName = "HMMGenerate";

#include <mlpack/core.hpp>
#include <mlpack/core/util/mlpack_main.hpp>
#include "test_helper.hpp"
#include <mlpack/methods/hmm/hmm_model.hpp>
#include <mlpack/methods/hmm/hmm.hpp>
#include <mlpack/methods/hmm/hmm_generate_main.cpp>

#include <boost/test/unit_test.hpp>
#include "../test_tools.hpp"

#include "hmm_test_utils.hpp"

using namespace mlpack;

struct HMMGenerateTestFixture
{
 public:
  HMMGenerateTestFixture()
  {
    // Cache in the options for this program.
    CLI::RestoreSettings(testName);
  }

  ~HMMGenerateTestFixture()
  {
    // Clear the settings.
    bindings::tests::CleanMemory();
    CLI::ClearSettings();
  }
};

BOOST_FIXTURE_TEST_SUITE(HMMGenerateMainTest, HMMGenerateTestFixture);

BOOST_AUTO_TEST_CASE(HMMGenerateDiscreteHMMCheckDimensionsTest)
{
  // Load data to train a discrete HMM model with.
  arma::mat inp;
  data::Load("obs1.csv", inp);
  std::vector<arma::mat> trainSeq = {inp};

  // Initialize and train a discrete HMM model.
  HMMModel* h = new HMMModel(DiscreteHMM);
  h->PerformAction<InitHMMModel, std::vector<arma::mat>>(&trainSeq);
  h->PerformAction<TrainHMMModel, std::vector<arma::mat>>(&trainSeq);

  // Now that we have a trained HMM model, we can use it to generate a sequence
  // of states and observations - using the hmm_generate utility.
  // Load the input model to be used for inference and the length of sequence
  // to be generated.
  int length = 3;
  SetInputParam("model", h);
  SetInputParam("length", length);

  // Call to hmm_generate_main.
  mlpackMain();

  // Get the generated observation sequence. Ensure that the generated sequence
  // has the correct length (as provided in the input).
  arma::mat obsSeq = CLI::GetParam<arma::mat>("output");
  BOOST_REQUIRE_EQUAL(obsSeq.n_cols, (size_t)length);
  BOOST_REQUIRE_EQUAL(obsSeq.n_rows, (size_t)1);
  BOOST_REQUIRE_EQUAL(obsSeq.n_elem, (size_t)length);

  // Get the generated state sequence. Ensure that the generated sequence
  // has the correct length (as provided in the input).
  arma::Mat<size_t> stateSeq = CLI::GetParam<arma::Mat<size_t>>("state");
  BOOST_REQUIRE_EQUAL(stateSeq.n_cols, (size_t)length);
  BOOST_REQUIRE_EQUAL(stateSeq.n_rows, (size_t)1);
  BOOST_REQUIRE_EQUAL(stateSeq.n_elem, (size_t)length);
}

BOOST_AUTO_TEST_CASE(HMMGenerateGaussianHMMCheckDimensionsTest)
{
  // Load data to train a gaussian HMM model with.
  arma::mat inp;
  data::Load("obs1.csv", inp);
  std::vector<arma::mat> trainSeq = {inp};

  // Initialize and train a gaussian HMM model.
  HMMModel* h = new HMMModel(GaussianHMM);
  h->PerformAction<InitHMMModel, std::vector<arma::mat>>(&trainSeq);
  h->PerformAction<TrainHMMModel, std::vector<arma::mat>>(&trainSeq);

  // Now that we have a trained HMM model, we can use it to generate a sequence
  // of states and observations - using the hmm_generate utility.
  // Load the input model to be used for inference and the length of sequence
  // to be generated.
  int length = 3;
  SetInputParam("model", h);
  SetInputParam("length", length);

  // Call to hmm_generate_main.
  mlpackMain();

  // Get the generated observation sequence. Ensure that the generated sequence
  // has the correct length (as provided in the input).
  arma::mat obsSeq = CLI::GetParam<arma::mat>("output");
  BOOST_REQUIRE_EQUAL(obsSeq.n_cols, (size_t)length);
  BOOST_REQUIRE_EQUAL(obsSeq.n_rows, (size_t)1);
  BOOST_REQUIRE_EQUAL(obsSeq.n_elem, (size_t)length);

  // Get the generated state sequence. Ensure that the generated sequence
  // has the correct length (as provided in the input).
  arma::Mat<size_t> stateSeq = CLI::GetParam<arma::Mat<size_t>>("state");
  BOOST_REQUIRE_EQUAL(stateSeq.n_cols, (size_t)length);
  BOOST_REQUIRE_EQUAL(stateSeq.n_rows, (size_t)1);
  BOOST_REQUIRE_EQUAL(stateSeq.n_elem, (size_t)length);
}

BOOST_AUTO_TEST_CASE(HMMGenerateGMMHMMCheckDimensionsTest)
{
  // Initialize and train a GMM HMM model.
  HMMModel* h = new HMMModel(GaussianMixtureModelHMM);
  *(h->GMMHMM()) = HMM<GMM>(2, GMM(2, 2));

  // Manually set the components.
  h->GMMHMM()->Transition() = arma::mat("0.40 0.60; 0.60 0.40");
  h->GMMHMM()->Emission().resize(2);
  h->GMMHMM()->Emission()[0] = GMM(2, 2);
  h->GMMHMM()->Emission()[0].Weights() = arma::vec("0.3 0.7");
  h->GMMHMM()->Emission()[0].Component(0) = GaussianDistribution("4.25 3.10",
      "1.00 0.20; 0.20 0.89");
  h->GMMHMM()->Emission()[0].Component(1) = GaussianDistribution("7.10 5.01",
      "1.00 0.00; 0.00 1.01");
  h->GMMHMM()->Emission()[1] = GMM(2, 2);
  h->GMMHMM()->Emission()[1].Weights() = arma::vec("0.20 0.80");
  h->GMMHMM()->Emission()[1].Component(0) = GaussianDistribution("-3.00 -6.12",
      "1.00 0.00; 0.00 1.00");
  h->GMMHMM()->Emission()[1].Component(1) = GaussianDistribution("-4.25 -2.12",
      "1.50 0.60; 0.60 1.20");

  // Now that we have a trained HMM model, we can use it to generate a sequence
  // of states and observations - using the hmm_generate utility.
  // Load the input model to be used for inference and the length of sequence
  // to be generated.
  int length = 3;
  SetInputParam("model", h);
  SetInputParam("length", length);

  // Call to hmm_generate_main
  mlpackMain();

  // Get the generated observation sequence. Ensure that the generated sequence
  // has the correct length (as provided in the input).
  arma::mat obsSeq = CLI::GetParam<arma::mat>("output");
  BOOST_REQUIRE_EQUAL(obsSeq.n_cols, (size_t) length);
  BOOST_REQUIRE_EQUAL(obsSeq.n_rows, (size_t) 2);
  BOOST_REQUIRE_EQUAL(obsSeq.n_elem, (size_t) (length*2));

  // Get the generated state sequence. Ensure that the generated sequence
  // has the correct length (as provided in the input).
  arma::Mat<size_t> stateSeq = CLI::GetParam<arma::Mat<size_t>>("state");
  BOOST_REQUIRE_EQUAL(stateSeq.n_cols, (size_t) length);
  BOOST_REQUIRE_EQUAL(stateSeq.n_rows, (size_t) 1);
  BOOST_REQUIRE_EQUAL(stateSeq.n_elem, (size_t) length);
}

BOOST_AUTO_TEST_CASE(HMMGenerateLengthPositiveTest)
{
  // Load data to train a Gaussian Mixture Model HMM model with.
  arma::mat inp;
  data::Load("obs1.csv", inp);
  std::vector<arma::mat> trainSeq = {inp};

  // Initialize and train a HMM model.
  HMMModel* h = new HMMModel(DiscreteHMM);
  h->PerformAction<InitHMMModel, std::vector<arma::mat>>(&trainSeq);
  h->PerformAction<TrainHMMModel, std::vector<arma::mat>>(&trainSeq);

  // Set the params for the hmm_generate invocation
  // Note that the length is negative - we expect that a runtime error will be
  // raised in the call to hmm_generate_main
  int length = -3; // Invalid
  SetInputParam("model", h);
  SetInputParam("length", length);

  Log::Fatal.ignoreInput = true;
  BOOST_REQUIRE_THROW(mlpackMain(), std::runtime_error);
  Log::Fatal.ignoreInput = false;
}

BOOST_AUTO_TEST_CASE(HMMGenerateValidStartStateTest)
{
  // Load data to train a Gaussian Mixture Model HMM model with.
  arma::mat inp;
  data::Load("obs1.csv", inp);
  std::vector<arma::mat> trainSeq = {inp};

  // Initialize and train a HMM model.
  HMMModel* h = new HMMModel(DiscreteHMM);
  h->PerformAction<InitHMMModel, std::vector<arma::mat>>(&trainSeq);
  h->PerformAction<TrainHMMModel, std::vector<arma::mat>>(&trainSeq);

  // Set the params for the hmm_generate invocation
  // Note that the start state is invalid - we expect that a runtime error will
  // be raised in the call to hmm_generate_main
  int length = 3;
  int startState = 2; // Invalid
  SetInputParam("model", h);
  SetInputParam("length", length);
  SetInputParam("start_state", startState);

  Log::Fatal.ignoreInput = true;
  BOOST_REQUIRE_THROW(mlpackMain(), std::runtime_error);
  Log::Fatal.ignoreInput = false;
}

BOOST_AUTO_TEST_SUITE_END();
