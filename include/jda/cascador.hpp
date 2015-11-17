#ifndef CASCADOR_HPP_
#define CASCADOR_HPP_

#include <vector>

namespace jda {

// pre-define
class DataSet;
class BoostCart;

/*!
 * \breif JoinCascador for face classification and landmark regression
 */
class JoinCascador {
public:
  JoinCascador();
  ~JoinCascador();
  /*!
   * \breif Initialize JionCascador
   * \param T   stages, should be `Config::T`
   */
  void Initialize(int T);

public:
  /*!
   * \breif Train JoinCascador
   *  See Full Algorithm on paper `Algorithm 3`
   */
  void Train(DataSet& pos, DataSet& neg);
  /*!
   * \breif Snapshot current model
   *  Save all model parameters currently trained, this will be used for `Resume`.
   *  A model file will be save at `../model/jda_tmp_{%time%}_{stage}.model`,
   *  `%time%` is the saving time point format `%Y%m%D-%H%M%S` like `20151011-103625`
   *  `stage` is current stage range in [1..c.T]
   */
  static void Snapshot(const JoinCascador& joincascador);
  /*!
   * \breif Resume the Training Status
   *  Resume the Training Status from a Snapshot model file. We will load the model
   *  parameters and check if the global parameters are the same as `Config`, if not,
   *  the program will be terminateed. After the parameters loaded, Positive Samples
   *  will be processed to generate the Positive DataSet, the Negative DataSet will
   *  be generated by hard negative mining.
   *
   * \param stage   which stage to start from, range in [2, c.T]
   * \param fd      file discriptor of the model file
   */
  static void Resume(JoinCascador& joincascador, int stage, FILE* fd);
  /*!
   * \breif Write parameters to a binary file
   * \param   file discriptor of the model file
   */
  void SerializeTo(FILE* fd);
  /*!
   * \breif Read parameters from a binary file
   * \param fd    file discriptor of the model file
   */
  void SerializeFrom(FILE* fd);

public:
  /*!
   * \breif Validate a region whether a face or not
   *  In training state, we use this function for hard negative mining based on
   *  the training status. In testing state, we just go through all carts to get
   *  a face score for this region. The training status is based on `current_stage_idx`
   *  and `current_cart_idx`.
   *
   * \param img     img
   * \param score   classification score of this image
   * \param shape   shape on this image
   * \return        whether a face or not
   */
  bool Validate(const cv::Mat& img, double& score, cv::Mat_<double>& shape) const;
  /*!
   * \breif Detect faces in a gray image
   *  Currently using Sliding Window to search face regions and Non-Maximum Suppression
   *  to group the regions, represented by cv::Rect. All shapes will be relocated in the
   *  original image.
   *
   * \note the interface may change later
   *
   * \param img       gray image
   * \param rects     face locations
   * \param scores    score of faces
   * \param shapes    shape of faces
   * \return          number of faces
   */
  int Detect(const cv::Mat& img, std::vector<cv::Rect>& rects, std::vector<double>& scores, \
             std::vector<cv::Mat_<double> >& shapes);

public:
  /*! \breif number of stages */
  int T;
  /*! \breif mean shape of positive training data */
  cv::Mat_<double> mean_shape;
  /*! \breif carts */
  std::vector<BoostCart> btcarts;

  /*! \breif training status */
  int current_stage_idx; // range in [0, c.T)
  int current_cart_idx; // range in [0, c.K)
  /*! \breif training data */
  DataSet* pos;
  DataSet* neg;
};

} // namespace jda

#endif // CASCADOR_HPP_
