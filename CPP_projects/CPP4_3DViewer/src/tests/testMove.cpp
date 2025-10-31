#include "tests_run.hpp"

TEST(Suite_move_model, test_1) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.moveModel(10.0, 2.0, 1.5);
  EXPECT_NEAR(model.getVertices()[0].x, 12.712726, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, 0.045698, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, -1.165440, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, 3.971338, 1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, 3.63977, 1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, 2.864798, 1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_move_model, test_2) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.moveModel(0.667, -8.0, 2.6);
  EXPECT_NEAR(model.getVertices()[0].x, 3.379726, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, -9.954302, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, -0.065440, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, -5.361662,
              1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, -6.36023, 1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, 3.964798, 1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_move_model, test_3) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.moveModel(3.75, -3.667, -0.875);
  EXPECT_NEAR(model.getVertices()[0].x, 6.462726, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, -5.621302, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, -3.540440, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, -2.278662,
              1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, -2.02723, 1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, 0.489798, 1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_move_model, test_4) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.moveModel(2.75, 1.667, -0.8);
  EXPECT_NEAR(model.getVertices()[0].x, 5.462726, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, -0.287302, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, -3.465440, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, -3.278662,
              1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, 3.30677, 1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, 0.564798, 1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_move_model, test_5) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.moveModel(1.5, -6.0, 3.2);
  EXPECT_NEAR(model.getVertices()[0].x, 4.212726, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, -7.954302, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, 0.534560, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, -4.528662,
              1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, -4.36023, 1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, 4.564798, 1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_move_model, test_6) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.moveModel(0.667, 1.0, 2.429);
  EXPECT_NEAR(model.getVertices()[0].x, 3.379726, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, -0.954302, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, -0.236440, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, -5.361662,
              1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, 2.63977, 1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, 3.793798, 1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_move_model, test_7) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.moveModel(2.333, 8.0, 9.0);
  EXPECT_NEAR(model.getVertices()[0].x, 5.045726, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, 6.045698, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, 6.334560, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, -3.695662,
              1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, 9.63977, 1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, 10.364798,
              1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_move_model, test_8) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.moveModel(6.5, -4.286, -2.833);
  EXPECT_NEAR(model.getVertices()[0].x, 9.212726, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, -6.240302, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, -5.498440, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, 0.471338, 1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, -2.64623, 1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, -1.468202,
              1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_move_model, test_9) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.moveModel(-1.667, -2.5, -0.429);
  EXPECT_NEAR(model.getVertices()[0].x, 1.045726, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, -4.454302, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, -3.094440, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, -7.695662,
              1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, -0.86023, 1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, 0.935798, 1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_move_model, test_10) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.moveModel(-3, -10.333, -0.25);
  EXPECT_NEAR(model.getVertices()[0].x, -0.287274, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, -12.287302, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, -2.915440, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, -9.028662,
              1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, -8.69323, 1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, 1.114798, 1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_move_model, test_11) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.moveModel(3.5, 14.333, -0.143);
  EXPECT_NEAR(model.getVertices()[0].x, 6.212726, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, 12.378698, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, -2.808440, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, -2.528662,
              1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, 15.97277, 1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, 1.221798, 1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_move_model, test_12) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.moveModel(-2.667, 12.667, 9.0);
  EXPECT_NEAR(model.getVertices()[0].x, 0.045726, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, 10.712698, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, 6.334560, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, -8.695662,
              1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, 14.30677, 1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, 10.364798,
              1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_move_model, test_13) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.moveModel(0.8, -1.25, -2.833);
  EXPECT_NEAR(model.getVertices()[0].x, 3.512726, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, -3.204302, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, -5.498440, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, -5.228662,
              1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, 0.38977, 1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, -1.468202,
              1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_move_model, test_14) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.moveModel(2.0, 15.0, -0.625);
  EXPECT_NEAR(model.getVertices()[0].x, 4.712726, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, 13.045698, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, -3.290440, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, -4.028662,
              1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, 16.63977, 1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, 0.739798, 1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_move_model, test_15) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.moveModel(6.5, 8.25, 3.714);
  EXPECT_NEAR(model.getVertices()[0].x, 9.212726, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, 6.295698, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, 1.048560, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, 0.471338, 1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, 9.88977, 1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, 5.078798, 1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_move_model, test_16) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.moveModel(-1.25, -2.5, 6.0);
  EXPECT_NEAR(model.getVertices()[0].x, 1.462726, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, -4.454302, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, 3.334560, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, -7.278662,
              1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, -0.86023, 1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, 7.364798, 1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_move_model, test_17) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.moveModel(-0.6, 5.167, -2.5);
  EXPECT_NEAR(model.getVertices()[0].x, 2.112726, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, 3.212698, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, -5.165440, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, -6.628662,
              1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, 6.80677, 1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, -1.135202,
              1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_move_model, test_18) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.moveModel(-1.75, -1.75, 3.714);
  EXPECT_NEAR(model.getVertices()[0].x, 0.962726, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, -3.704302, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, 1.048560, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, -7.778662,
              1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, -0.11023, 1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, 5.078798, 1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_move_model, test_19) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.moveModel(-1.0, 6.333, 1.75);
  EXPECT_NEAR(model.getVertices()[0].x, 1.712726, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, 4.378698, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, -0.915440, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, -7.028662,
              1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, 7.97277, 1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, 3.114798, 1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_move_model, test_20) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.moveModel(5.0, -19.0, 1.167);
  EXPECT_NEAR(model.getVertices()[0].x, 7.712726, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, -20.954302, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, -1.498440, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, -1.028662,
              1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, -17.36023,
              1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, 2.531798, 1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_move_model, test_21) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.moveModel(3.2, -0.4, 6.0);
  EXPECT_NEAR(model.getVertices()[0].x, 5.912726, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, -2.354302, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, 3.334560, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, -2.828662,
              1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, 1.23977, 1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, 7.364798, 1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
