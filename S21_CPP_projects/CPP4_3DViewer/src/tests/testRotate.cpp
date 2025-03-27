#include "tests_run.hpp"

TEST(Suite_rotate_model, test_1) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.rotateModel(20, 0, 0);
  EXPECT_NEAR(model.getVertices()[0].x, 2.712726, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, -0.924809, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, -3.173105, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, -6.028662,
              1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, 1.074091, 1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, 1.843325, 1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_rotate_model, test_2) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.rotateModel(0, 50, 0);
  EXPECT_NEAR(model.getVertices()[0].x, -0.298139, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, -1.954302, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, -3.791380, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, -2.829653,
              1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, 1.63977, 1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, 5.495498, 1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_rotate_model, test_3) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.rotateModel(0, 0, 185);
  EXPECT_NEAR(model.getVertices()[0].x, -2.872732, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, 1.710435, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, -2.66544, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, 6.148636, 1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, -1.108097,
              1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, 1.364798, 1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_rotate_model, test_4) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.rotateModel(-274, 216, -164);
  EXPECT_NEAR(model.getVertices()[0].x, 1.598378, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, -2.165955, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, 3.322134, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, -4.054066,
              1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, 0.134861, 1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, -4.94395, 1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_rotate_model, test_5) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.rotateModel(333, -86, 240);
  EXPECT_NEAR(model.getVertices()[0].x, -3.392618, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, 0.026573, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, 2.602342, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, 2.247393, 1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, -0.268703,
              1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, -5.981079,
              1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_rotate_model, test_6) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.rotateModel(306, 184, 257);
  EXPECT_NEAR(model.getVertices()[0].x, -2.611417, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, 3.381221, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, 0.174908, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, 0.653896, 1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, -6.360675,
              1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, 0.102578, 1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_rotate_model, test_7) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.rotateModel(-159, 74, 122);
  EXPECT_NEAR(model.getVertices()[0].x, -2.757762, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, 2.772923, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, -1.728698, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, 2.712419, 1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, -2.374899,
              1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, 5.281943, 1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_rotate_model, test_8) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.rotateModel(118, -146, -358);
  EXPECT_NEAR(model.getVertices()[0].x, -2.096729, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, 3.199707, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, 1.910065, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, 4.612817, 1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, -1.814991,
              1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, -4.040299,
              1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_rotate_model, test_9) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.rotateModel(62, -263, -180);
  EXPECT_NEAR(model.getVertices()[0].x, 3.285303, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, -1.435955, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, -2.329714, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, -2.807706,
              1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, 0.43522, 1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, 5.729193, 1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_rotate_model, test_10) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.rotateModel(139, 91, -4);
  EXPECT_NEAR(model.getVertices()[0].x, 0.905248, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, 3.168187, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, -2.725044, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, 0.001813, 1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, -2.138273,
              1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, 6.026945, 1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_rotate_model, test_11) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.rotateModel(-186, 273, 264);
  EXPECT_NEAR(model.getVertices()[0].x, 2.450581, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, 2.056342, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, 2.837051, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, -1.854544,
              1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, -0.678643,
              1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, -6.082466,
              1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_rotate_model, test_12) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.rotateModel(254, 74, 69);
  EXPECT_NEAR(model.getVertices()[0].x, 3.057309, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, 2.318111, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, -1.887319, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, -2.070924,
              1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, -2.995323,
              1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, 5.256957, 1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_rotate_model, test_13) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.rotateModel(-113, 254, -18);
  EXPECT_NEAR(model.getVertices()[0].x, -3.830101, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, -0.532437, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, 1.824715, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, 3.638075, 1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, -0.534808,
              1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, -5.232081,
              1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_rotate_model, test_14) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.rotateModel(56, 159, 16);
  EXPECT_NEAR(model.getVertices()[0].x, -3.813891, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, 0.068314, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, 1.931920, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, 6.200553, 1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, 1.554814, 1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, 0.178846, 1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_rotate_model, test_15) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.rotateModel(-220, 138, 252);
  EXPECT_NEAR(model.getVertices()[0].x, 3.513779, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, 0.425245, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, -2.399016, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, -3.415207,
              1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, -3.60706, 1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, 4.027625, 1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_rotate_model, test_16) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.rotateModel(-167, -60, -292);
  EXPECT_NEAR(model.getVertices()[0].x, -1.686698, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, -0.692084, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, 3.867663, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, 0.618642, 1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, -1.91437, 1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, -6.070318,
              1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_rotate_model, test_17) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.rotateModel(30, 103, -78);
  EXPECT_NEAR(model.getVertices()[0].x, -1.144352, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, 3.653425, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, -1.904125, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, 1.409061, 1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, -3.08105, 1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, 5.423833, 1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_rotate_model, test_18) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.rotateModel(-322, 259, 170);
  EXPECT_NEAR(model.getVertices()[0].x, -2.701410, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, 0.373776, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, 3.293239, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, 0.804298, 1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, -0.600694,
              1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, -6.315739,
              1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_rotate_model, test_19) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.rotateModel(206, 104, 336);
  EXPECT_NEAR(model.getVertices()[0].x, 2.522606, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, -0.479421, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, -3.418971, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, -0.748244,
              1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, -0.625244,
              1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, 6.320244, 1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_rotate_model, test_20) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.rotateModel(-348, -52, -1);
  EXPECT_NEAR(model.getVertices()[0].x, 4.020498, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, -1.427804, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, 0.282352, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, -5.008436,
              1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, 1.407803, 1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, -3.718863,
              1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_rotate_model, test_21) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.rotateModel(235, 300, 98);
  EXPECT_NEAR(model.getVertices()[0].x, 1.240565, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, -1.192997, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, 3.914142, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, -0.012449,
              1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, -1.186408,
              1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, -6.283993,
              1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_rotate_model, test_22) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.rotateModel(-331, -202, -282);
  EXPECT_NEAR(model.getVertices()[0].x, -0.370375, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, -3.748327, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, 2.023763, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, 0.561419, 1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, 6.356828, 1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, 0.414527, 1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
TEST(Suite_rotate_model, test_23) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  model.rotateModel(-177, -290, 35);
  EXPECT_NEAR(model.getVertices()[0].x, 1.848267, 1e-6)
      << "wrong first line vertex x value";
  EXPECT_NEAR(model.getVertices()[0].y, 3.506367, 1e-6)
      << "wrong first line vertex y value";
  EXPECT_NEAR(model.getVertices()[0].z, -1.603762, 1e-6)
      << "wrong first line vertex z value";

  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].x, -1.905928,
              1e-6)
      << "wrong last line vertex x value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].y, -3.246394,
              1e-6)
      << "wrong last line vertex y value";
  EXPECT_NEAR(model.getVertices()[model.getNumVertices() - 1].z, 5.169589, 1e-6)
      << "wrong last line vertex z value";

  model.freeModel();
}
