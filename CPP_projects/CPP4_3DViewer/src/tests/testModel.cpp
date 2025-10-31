#include "tests_run.hpp"

TEST(Suite_objModel_class, test_1) {
  ObjModel &model = ObjModel::getInstance();

  EXPECT_TRUE(model.getVertices().empty()) << "model.vertices init failed";
  EXPECT_TRUE(model.getEdges().empty()) << "model.edges init failed";

  EXPECT_EQ(model.getNumVertices(), 0) << "model.numVertices init failed";
  EXPECT_EQ(model.getNumEdges(), 0) << "model.numEdges init failed";

  EXPECT_EQ(model.getMaxMin().maxX, 0) << "model.maxMin.maxX init failed";
  EXPECT_EQ(model.getMaxMin().maxY, 0) << "model.maxMin.maxY init failed";
  EXPECT_EQ(model.getMaxMin().maxZ, 0) << "model.maxMin.maxZ init failed";
  EXPECT_EQ(model.getMaxMin().minX, 0) << "model.maxMin.minX init failed";
  EXPECT_EQ(model.getMaxMin().minY, 0) << "model.maxMin.minY init failed";
  EXPECT_EQ(model.getMaxMin().minZ, 0) << "model.maxMin.minZ init failed";

  EXPECT_EQ(model.getCurrentScale(), 1) << "model.currentScale init failed";

  EXPECT_EQ(model.getTransformationMatrix().size(), 4)
      << "transformationMatrix size is not 0 after initialization";
  EXPECT_EQ(model.getRotationMatrixX().size(), 3)
      << "RotationMatrixX size is not 0 after initialization";
  EXPECT_EQ(model.getRotationMatrixY().size(), 3)
      << "RotationMatrixY size is not 0 after initialization";
  EXPECT_EQ(model.getRotationMatrixZ().size(), 3)
      << "RotationMatrixZ size is not 0 after initialization";
  EXPECT_EQ(model.getMoveVector().size(), 3)
      << "MoveVector size is not 0 after initialization";

  model.freeModel();
}
TEST(Suite_objModel_class, test_2) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/Men.obj";
  loadObjModel(filename);

  EXPECT_EQ(model.getNumVertices(), 24461) << "wrong vertices amount parsed";
  EXPECT_EQ(model.getNumEdges(), 24459) << "wrong edges amount parsed";
  EXPECT_EQ(model.getVertices()[0].x, -1.3143)
      << "wrong first line vertex x value";
  EXPECT_EQ(model.getVertices()[0].y, 15.0686)
      << "wrong first line vertex y value";
  EXPECT_EQ(model.getVertices()[0].z, -1.6458)
      << "wrong first line vertex z value";
  EXPECT_EQ(model.getVertices()[model.getNumVertices() - 1].x, 5.7584)
      << "wrong last line vertex x value";
  EXPECT_EQ(model.getVertices()[model.getNumVertices() - 1].y, 9.1904)
      << "wrong last line vertex y value";
  EXPECT_EQ(model.getVertices()[model.getNumVertices() - 1].z, -0.3476)
      << "wrong last line vertex z value";

  EXPECT_EQ(model.getEdges()[0].vertexIndex[0], 1.0 - 1)
      << "wrong polygon value [first][0]";
  EXPECT_EQ(model.getEdges()[0].vertexIndex[1], 2.0 - 1)
      << "wrong polygon value [first][1]";

  EXPECT_EQ(model.getEdges()[0].vertexIndex[model.getEdges()[0].amountPoly - 2],
            4 - 1)
      << "wrong polygon value [first][last-1]";
  EXPECT_EQ(model.getEdges()[0].vertexIndex[model.getEdges()[0].amountPoly - 1],
            1.0 - 1)
      << "wrong polygon value [first][last]";

  EXPECT_EQ(model.getEdges()[model.getNumEdges() - 1].vertexIndex[0], 21869 - 1)
      << "wrong polygon value [last][0]";
  EXPECT_EQ(model.getEdges()[model.getNumEdges() - 1].vertexIndex[1], 21868 - 1)
      << "wrong polygon value [last][1]";

  EXPECT_EQ(
      model.getEdges()[model.getNumEdges() - 1]
          .vertexIndex[model.getEdges()[model.getNumEdges() - 1].amountPoly -
                       2],
      19887 - 1)
      << "wrong polygon value [last][last - 1]";
  EXPECT_EQ(
      model.getEdges()[model.getNumEdges() - 1]
          .vertexIndex[model.getEdges()[model.getNumEdges() - 1].amountPoly -
                       1],
      21869 - 1)
      << "wrong polygon value [last][last]";

  model.freeModel();
}
TEST(Suite_objModel_class, test_3) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/teapot.obj";
  loadObjModel(filename);

  EXPECT_EQ(model.getNumVertices(), 7850) << "wrong vertices amount parsed";
  EXPECT_EQ(model.getNumEdges(), 8028) << "wrong edges amount parsed";
  EXPECT_EQ(model.getVertices()[0].x, 33.647778)
      << "wrong first line vertex x value";
  EXPECT_EQ(model.getVertices()[0].y, 59.807484)
      << "wrong first line vertex y value";
  EXPECT_EQ(model.getVertices()[0].z, 4.662212)
      << "wrong first line vertex z value";
  EXPECT_EQ(model.getVertices()[model.getNumVertices() - 1].x, 0.000000)
      << "wrong last line vertex x value";
  EXPECT_EQ(model.getVertices()[model.getNumVertices() - 1].y, 66.219269)
      << "wrong last line vertex y value";
  EXPECT_EQ(model.getVertices()[model.getNumVertices() - 1].z, -0.000000)
      << "wrong last line vertex z value";

  EXPECT_EQ(model.getEdges()[0].vertexIndex[0], 2 - 1)
      << "wrong polygon value [first][0]";
  EXPECT_EQ(model.getEdges()[0].vertexIndex[1], 1.0 - 1)
      << "wrong polygon value [first][1]";

  EXPECT_EQ(model.getEdges()[0].vertexIndex[model.getEdges()[0].amountPoly - 2],
            1537 - 1)
      << "wrong polygon value [first][last-1]";
  EXPECT_EQ(model.getEdges()[0].vertexIndex[model.getEdges()[0].amountPoly - 1],
            2 - 1)
      << "wrong polygon value [first][last]";

  EXPECT_EQ(model.getEdges()[model.getNumEdges() - 1].vertexIndex[0], 7273 - 1)
      << "wrong polygon value [last][0]";
  EXPECT_EQ(model.getEdges()[model.getNumEdges() - 1].vertexIndex[1], 7226 - 1)
      << "wrong polygon value [last][1]";

  EXPECT_EQ(
      model.getEdges()[model.getNumEdges() - 1]
          .vertexIndex[model.getEdges()[model.getNumEdges() - 1].amountPoly -
                       2],
      7850 - 1)
      << "wrong polygon value [last][last - 1]";
  EXPECT_EQ(
      model.getEdges()[model.getNumEdges() - 1]
          .vertexIndex[model.getEdges()[model.getNumEdges() - 1].amountPoly -
                       1],
      7273 - 1)
      << "wrong polygon value [last][last]";

  model.freeModel();
}
TEST(Suite_objModel_class, test_4) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/Cat_v1.obj";
  loadObjModel(filename);

  EXPECT_EQ(model.getNumVertices(), 35290) << "wrong vertices amount parsed";
  EXPECT_EQ(model.getNumEdges(), 35288) << "wrong edges amount parsed";
  EXPECT_EQ(model.getVertices()[0].x, 0.6134)
      << "wrong first line vertex x value";
  EXPECT_EQ(model.getVertices()[0].y, -21.9357)
      << "wrong first line vertex y value";
  EXPECT_EQ(model.getVertices()[0].z, 31.4441)
      << "wrong first line vertex z value";
  EXPECT_EQ(model.getVertices()[model.getNumVertices() - 1].x, -0.3722)
      << "wrong last line vertex x value";
  EXPECT_EQ(model.getVertices()[model.getNumVertices() - 1].y, -15.0839)
      << "wrong last line vertex y value";
  EXPECT_EQ(model.getVertices()[model.getNumVertices() - 1].z, 15.1684)
      << "wrong last line vertex z value";

  EXPECT_EQ(model.getEdges()[0].vertexIndex[0], 1.0 - 1)
      << "wrong polygon value [first][0]";
  EXPECT_EQ(model.getEdges()[0].vertexIndex[1], 2 - 1)
      << "wrong polygon value [first][1]";

  EXPECT_EQ(model.getEdges()[0].vertexIndex[model.getEdges()[0].amountPoly - 2],
            4 - 1)
      << "wrong polygon value [first][last-1]";
  EXPECT_EQ(model.getEdges()[0].vertexIndex[model.getEdges()[0].amountPoly - 1],
            1.0 - 1)
      << "wrong polygon value [first][last]";

  EXPECT_EQ(model.getEdges()[model.getNumEdges() - 1].vertexIndex[0], 35289 - 1)
      << "wrong polygon value [last][0]";
  EXPECT_EQ(model.getEdges()[model.getNumEdges() - 1].vertexIndex[1], 35290 - 1)
      << "wrong polygon value [last][1]";

  EXPECT_EQ(
      model.getEdges()[model.getNumEdges() - 1]
          .vertexIndex[model.getEdges()[model.getNumEdges() - 1].amountPoly -
                       2],
      35285 - 1)
      << "wrong polygon value [last][last - 1]";
  EXPECT_EQ(
      model.getEdges()[model.getNumEdges() - 1]
          .vertexIndex[model.getEdges()[model.getNumEdges() - 1].amountPoly -
                       1],
      35289 - 1)
      << "wrong polygon value [last][last]";

  model.freeModel();
}
TEST(Suite_objModel_class, test_5) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/trumpet.obj";
  loadObjModel(filename);

  EXPECT_EQ(model.getNumVertices(), 11908) << "wrong vertices amount parsed";
  EXPECT_EQ(model.getNumEdges(), 11362) << "wrong edges amount parsed";
  EXPECT_EQ(model.getVertices()[0].x, 9.804816)
      << "wrong first line vertex x value";
  EXPECT_EQ(model.getVertices()[0].y, -554.003418)
      << "wrong first line vertex y value";
  EXPECT_EQ(model.getVertices()[0].z, 228.321411)
      << "wrong first line vertex z value";
  EXPECT_EQ(model.getVertices()[model.getNumVertices() - 1].x, -51.853733)
      << "wrong last line vertex x value";
  EXPECT_EQ(model.getVertices()[model.getNumVertices() - 1].y, -403.707306)
      << "wrong last line vertex y value";
  EXPECT_EQ(model.getVertices()[model.getNumVertices() - 1].z, -164.112930)
      << "wrong last line vertex z value";

  EXPECT_EQ(model.getEdges()[0].vertexIndex[0], 40 - 1)
      << "wrong polygon value [first][0]";
  EXPECT_EQ(model.getEdges()[0].vertexIndex[1], 38 - 1)
      << "wrong polygon value [first][1]";

  EXPECT_EQ(model.getEdges()[0].vertexIndex[model.getEdges()[0].amountPoly - 2],
            2 - 1)
      << "wrong polygon value [first][last-1]";
  EXPECT_EQ(model.getEdges()[0].vertexIndex[model.getEdges()[0].amountPoly - 1],
            40 - 1)
      << "wrong polygon value [first][last]";

  EXPECT_EQ(model.getEdges()[model.getNumEdges() - 1].vertexIndex[0], 11866 - 1)
      << "wrong polygon value [last][0]";
  EXPECT_EQ(model.getEdges()[model.getNumEdges() - 1].vertexIndex[1], 11900 - 1)
      << "wrong polygon value [last][1]";

  EXPECT_EQ(
      model.getEdges()[model.getNumEdges() - 1]
          .vertexIndex[model.getEdges()[model.getNumEdges() - 1].amountPoly -
                       2],
      11867 - 1)
      << "wrong polygon value [last][last - 1]";
  EXPECT_EQ(
      model.getEdges()[model.getNumEdges() - 1]
          .vertexIndex[model.getEdges()[model.getNumEdges() - 1].amountPoly -
                       1],
      11866 - 1)
      << "wrong polygon value [last][last]";

  model.freeModel();
}

// WRONG OBJ FILES OR WITH ERRORS

TEST(Suite_objModel_class, test_6) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/only_for_tests/wr_test_1.obj";

  EXPECT_ANY_THROW(loadObjModel(filename));

  model.freeModel();
}
TEST(Suite_objModel_class, test_7) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/only_for_tests/wr_test_2.obj";
  EXPECT_ANY_THROW(loadObjModel(filename));
  model.freeModel();
}
TEST(Suite_objModel_class, test_8) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/only_for_tests/wr_test_3.obj";
  EXPECT_ANY_THROW(loadObjModel(filename));
  model.freeModel();
}
TEST(Suite_objModel_class, test_9) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/only_for_tests/wr_test_4.obj";
  loadObjModel(filename);
  EXPECT_EQ(model.getNumVertices(), 0) << "wrong vertices amount parsed";
  EXPECT_EQ(model.getNumEdges(), 0) << "wrong edges amount parsed";
  model.freeModel();
}
TEST(Suite_objModel_class, test_10) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/only_for_tests/wr_test_5.obj";
  loadObjModel(filename);
  EXPECT_EQ(model.getNumVertices(), 2) << "wrong vertices amount parsed";
  EXPECT_EQ(model.getNumEdges(), 2) << "wrong edges amount parsed";
  model.freeModel();
}
TEST(Suite_objModel_class, test_11) {
  ObjModel &model = ObjModel::getInstance();
  std::string filename = "3d_models/only_for_tests/wr_test_6.obj";
  EXPECT_ANY_THROW(loadObjModel(filename));
  model.freeModel();
}