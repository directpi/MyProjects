#include "tests_run.hpp"

TEST(Suite_center_align, test_1) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  double check_x = model.getVertices()[0].x;
  double check_y = model.getVertices()[0].y;
  double check_z = model.getVertices()[0].z;
  double xCenter = (model.getMaxMin().minX + model.getMaxMin().maxX) / 2.0;
  double yCenter = (model.getMaxMin().minY + model.getMaxMin().maxY) / 2.0;
  double zCenter = (model.getMaxMin().minZ + model.getMaxMin().maxZ) / 2.0;

  model.normalizeModel();
  double maxDimension =
      std::max(model.getMaxMin().maxX - model.getMaxMin().minX,
               std::max(model.getMaxMin().maxY - model.getMaxMin().minY,
                        model.getMaxMin().maxZ - model.getMaxMin().minZ));

  EXPECT_EQ(model.getVertices()[0].x, (check_x - xCenter) / maxDimension)
      << "X aligning failed";
  EXPECT_EQ(model.getVertices()[0].y, (check_y - yCenter) / maxDimension)
      << "Y aligning failed";
  EXPECT_EQ(model.getVertices()[0].z, (check_z - zCenter) / maxDimension)
      << "Z aligning failed";

  model.freeModel();
}
TEST(Suite_center_align, test_2) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/al.obj";
  loadObjModel(filename);

  double check_x = model.getVertices()[0].x;
  double check_y = model.getVertices()[0].y;
  double check_z = model.getVertices()[0].z;
  double xCenter = (model.getMaxMin().minX + model.getMaxMin().maxX) / 2.0;
  double yCenter = (model.getMaxMin().minY + model.getMaxMin().maxY) / 2.0;
  double zCenter = (model.getMaxMin().minZ + model.getMaxMin().maxZ) / 2.0;

  model.normalizeModel();
  double maxDimension =
      std::max(model.getMaxMin().maxX - model.getMaxMin().minX,
               std::max(model.getMaxMin().maxY - model.getMaxMin().minY,
                        model.getMaxMin().maxZ - model.getMaxMin().minZ));

  EXPECT_EQ(model.getVertices()[0].x, (check_x - xCenter) / maxDimension)
      << "X aligning failed";
  EXPECT_EQ(model.getVertices()[0].y, (check_y - yCenter) / maxDimension)
      << "Y aligning failed";
  EXPECT_EQ(model.getVertices()[0].z, (check_z - zCenter) / maxDimension)
      << "Z aligning failed";

  model.freeModel();
}
TEST(Suite_scale, test_1) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  Vertex vertex = model.getVertices()[0];
  double scale = 1.0;

  model.scaleModel(scale);

  EXPECT_NEAR(vertex.x * scale, model.getVertices()[0].x, 1e-6)
      << "wrong scale x value";
  EXPECT_NEAR(vertex.y * scale, model.getVertices()[0].y, 1e-6)
      << "wrong scale y value";
  EXPECT_NEAR(vertex.z * scale, model.getVertices()[0].z, 1e-6)
      << "wrong scale z value";

  model.freeModel();
}
TEST(Suite_scale, test_2) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  Vertex vertex = model.getVertices()[0];
  double scale = 2.0;

  model.scaleModel(scale);

  EXPECT_NEAR(vertex.x * scale, model.getVertices()[0].x, 1e-6)
      << "wrong scale x value";
  EXPECT_NEAR(vertex.y * scale, model.getVertices()[0].y, 1e-6)
      << "wrong scale y value";
  EXPECT_NEAR(vertex.z * scale, model.getVertices()[0].z, 1e-6)
      << "wrong scale z value";

  model.freeModel();
}
TEST(Suite_scale, test_3) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  Vertex vertex = model.getVertices()[0];
  double scale = 3.0;

  model.scaleModel(scale);

  EXPECT_NEAR(vertex.x * scale, model.getVertices()[0].x, 1e-6)
      << "wrong scale x value";
  EXPECT_NEAR(vertex.y * scale, model.getVertices()[0].y, 1e-6)
      << "wrong scale y value";
  EXPECT_NEAR(vertex.z * scale, model.getVertices()[0].z, 1e-6)
      << "wrong scale z value";

  model.freeModel();
}
TEST(Suite_scale, test_4) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  Vertex vertex = model.getVertices()[0];
  double scale = 0.9;

  model.scaleModel(scale);

  EXPECT_NEAR(vertex.x * scale, model.getVertices()[0].x, 1e-6)
      << "wrong scale x value";
  EXPECT_NEAR(vertex.y * scale, model.getVertices()[0].y, 1e-6)
      << "wrong scale y value";
  EXPECT_NEAR(vertex.z * scale, model.getVertices()[0].z, 1e-6)
      << "wrong scale z value";

  model.freeModel();
}
TEST(Suite_scale, test_5) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  Vertex vertex = model.getVertices()[0];
  double scale = 0.8;

  model.scaleModel(scale);

  EXPECT_NEAR(vertex.x * scale, model.getVertices()[0].x, 1e-6)
      << "wrong scale x value";
  EXPECT_NEAR(vertex.y * scale, model.getVertices()[0].y, 1e-6)
      << "wrong scale y value";
  EXPECT_NEAR(vertex.z * scale, model.getVertices()[0].z, 1e-6)
      << "wrong scale z value";

  model.freeModel();
}
TEST(Suite_scale, test_6) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  Vertex vertex = model.getVertices()[0];
  double scale = 0.1;

  model.scaleModel(scale);

  EXPECT_NEAR(vertex.x * scale, model.getVertices()[0].x, 1e-6)
      << "wrong scale x value";
  EXPECT_NEAR(vertex.y * scale, model.getVertices()[0].y, 1e-6)
      << "wrong scale y value";
  EXPECT_NEAR(vertex.z * scale, model.getVertices()[0].z, 1e-6)
      << "wrong scale z value";

  model.freeModel();
}
TEST(Suite_scale, test_7) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  Vertex vertex = model.getVertices()[0];
  double scale = 0.0000001;

  model.scaleModel(scale);

  EXPECT_NEAR(vertex.x * scale, model.getVertices()[0].x, 1e-6)
      << "wrong scale x value";
  EXPECT_NEAR(vertex.y * scale, model.getVertices()[0].y, 1e-6)
      << "wrong scale y value";
  EXPECT_NEAR(vertex.z * scale, model.getVertices()[0].z, 1e-6)
      << "wrong scale z value";

  model.freeModel();
}
TEST(Suite_scale, test_8) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/airboat.obj";
  loadObjModel(filename);

  double scale = 0.0;

  EXPECT_ANY_THROW(model.scaleModel(scale));

  model.freeModel();
}
