#ifndef DALI_DEMO_PBR_OBJ_LOADER_H
#define DALI_DEMO_PBR_OBJ_LOADER_H

/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

// EXTERNAL INCLUDES
#include <dali/public-api/rendering/geometry.h>
#include <limits>

using namespace Dali;

namespace PbrDemo
{
class ObjLoader
{
public:
  struct TriIndex
  {
    int pointIndex[3];
    int normalIndex[3];
    int textureIndex[3];
  };

  struct BoundingVolume
  {
    void Init()
    {
      pointMin = Vector3(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
      pointMax = Vector3(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
    }

    void ConsiderNewPointInVolume(const Vector3& position)
    {
      pointMin.x = std::min(position.x, pointMin.x);
      pointMin.y = std::min(position.y, pointMin.y);
      pointMin.z = std::min(position.z, pointMin.z);

      pointMax.x = std::max(position.x, pointMax.x);
      pointMax.y = std::max(position.y, pointMax.y);
      pointMax.z = std::max(position.z, pointMax.z);
    }

    Vector3 pointMin;
    Vector3 pointMax;
  };

  //Defines bit masks to declare which properties are needed by anyone requesting a geometry.
  enum ObjectProperties
  {
    TEXTURE_COORDINATES = 1 << 0,
    TANGENTS            = 1 << 1,
    BINORMALS           = 1 << 2
  };

  ObjLoader();
  virtual ~ObjLoader();

  bool IsSceneLoaded();
  bool IsMaterialLoaded();

  bool LoadObject(char* objBuffer, std::streampos fileSize);

  void LoadMaterial(char* objBuffer, std::streampos fileSize, std::string& diffuseTextureUrl, std::string& normalTextureUrl, std::string& glossTextureUrl);

  Geometry CreateGeometry(int objectProperties, bool useSoftNormals);

  Vector3 GetCenter();
  Vector3 GetSize();

  void ClearArrays();

  bool IsTexturePresent();
  bool IsDiffuseMapPresent();
  bool IsNormalMapPresent();
  bool IsSpecularMapPresent();

private:
  Dali::Vector<Vector3>  mPoints;
  Dali::Vector<Vector2>  mTextureUv;
  Dali::Vector<Vector2>  mTextureUv2;
  Dali::Vector<Vector3>  mNormals;
  Dali::Vector<Vector3>  mTangents;
  Dali::Vector<Vector3>  mBiTangents;
  Dali::Vector<TriIndex> mTriangles;

  BoundingVolume mSceneAABB;

  bool mSceneLoaded;
  bool mMaterialLoaded;
  bool mHasTextureUv;

  //Material file properties.
  bool mHasDiffuseMap;
  bool mHasNormalMap;
  bool mHasSpecularMap;

  /**
   * @brief Calculates normals for each point on a per-face basis.
   *
   * There are multiple normals per point, each corresponding to the normal of a face connecting to the point.
   *
   * @param[in] vertices The vertices of the object.
   * @param[in, out] triangles The triangles that form the faces. The normals of each triangle will be updated.
   * @param[in, out] normals The normals to be calculated.
   */
  void CalculateHardFaceNormals(const Dali::Vector<Vector3>& vertices,
                                Dali::Vector<TriIndex>&      triangles,
                                Dali::Vector<Vector3>&       normals);

  /**
   * @brief Calculates smoothed normals for each point.
   *
   * There is one normal per point, an average of the connecting faces.
   *
   * @param[in] vertices The vertices of the object.
   * @param[in, out] triangles The triangles that form the faces. The normals of each triangle will be updated.
   * @param[in, out] normals The normals to be calculated.
   */
  void CalculateSoftFaceNormals(const Dali::Vector<Vector3>& vertices,
                                Dali::Vector<TriIndex>&      triangles,
                                Dali::Vector<Vector3>&       normals);

  /**
   * @brief Calculates tangents and bitangents for each point of the object.
   *
   * These are calculated using the object's points, texture coordinates and normals, so these must be initialised first.
   */
  void CalculateTangentFrame();

  void CenterAndScale(bool center, Dali::Vector<Vector3>& points);

  /**
   * @brief Using the data loaded from the file, create arrays of data to be used in creating the geometry.
   *
   * @param[out] positions The positions of the vertices of the object.
   * @param[out] normals The normals of the vertices of the object.
   * @param[out] tangents The tangents of the vertices of the object.
   * @param[out] textures The texture coordinates of the vertices of the object.
   * @param[out] indices Indices of corresponding values to match triangles to their respective data.
   * @param[in] useSoftNormals Indicates whether we should average the normals at each point to smooth the surface or not.
   */
  void CreateGeometryArray(Dali::Vector<Vector3>&        positions,
                           Dali::Vector<Vector3>&        normals,
                           Dali::Vector<Vector3>&        tangents,
                           Dali::Vector<Vector2>&        textures,
                           Dali::Vector<unsigned short>& indices,
                           bool                          useSoftNormals);
};

} // namespace PbrDemo

#endif // DALI_DEMO_PBR_OBJ_LOADER_H
