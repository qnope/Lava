#include <gtest/gtest.h>
#include <Lava/Image/ImageDimension.h>

using namespace lava;
using namespace ImageDimension;

TEST(test_image_dimension, extent) {
    auto x = Dim1D{100};
    auto y = Dim2D{200, 300};
    auto z = Dim3D{400, 500, 600};

    {
        ASSERT_EQ(extent2DFromImageDimension(x), (vk::Extent2D{100, 1}));
        ASSERT_EQ(extent2DFromImageDimension(y), (vk::Extent2D{200, 300}));
        ASSERT_EQ(extent2DFromImageDimension(z), (vk::Extent2D{400, 500}));
    }

    {
        ASSERT_EQ(extent3DFromImageDimension(x), (vk::Extent3D{100, 1, 1}));
        ASSERT_EQ(extent3DFromImageDimension(y), (vk::Extent3D{200, 300, 1}));
        ASSERT_EQ(extent3DFromImageDimension(z), (vk::Extent3D{400, 500, 600}));
    }

    {
        ASSERT_EQ((imageDimensionFromExtent2D(vk::Extent2D{100, 200})), (image_dimension_t{Dim2D{100, 200}}));
        ASSERT_EQ((imageDimensionFromExtent3D(vk::Extent3D{300, 400, 500})), (image_dimension_t{Dim3D{300, 400, 500}}));
    }
}

TEST(test_image_dimension, mipmap_number) {
    {
        ASSERT_EQ(lava::getMaxMipmapLevels(Dim1D{1}), 1);
        ASSERT_EQ(lava::getMaxMipmapLevels(Dim1D{2}), 2);
        ASSERT_EQ(lava::getMaxMipmapLevels(Dim1D{4}), 3);
        ASSERT_EQ(lava::getMaxMipmapLevels(Dim1D{128}), 8);
        ASSERT_EQ(lava::getMaxMipmapLevels(Dim1D{64}), 7);
        ASSERT_EQ(lava::getMaxMipmapLevels(Dim1D{100}), 7);
    }

    {
        ASSERT_EQ(lava::getMaxMipmapLevels(Dim2D{128, 64}), 8);
        ASSERT_EQ(lava::getMaxMipmapLevels(Dim2D{64, 128}), 8);
        ASSERT_EQ(lava::getMaxMipmapLevels(Dim2D{128, 128}), 8);
        ASSERT_EQ(lava::getMaxMipmapLevels(Dim3D{128, 64, 1}), 8);
        ASSERT_EQ(lava::getMaxMipmapLevels(Dim3D{64, 1, 128}), 8);
        ASSERT_EQ(lava::getMaxMipmapLevels(Dim3D{16, 64, 32}), 7);
    }
}

TEST(test_image_dimension, mipmap_dimension) {
    {
        ASSERT_EQ((lava::getMipmapLevelDimension(Dim1D{128}, 7)), image_dimension_t{Dim1D{1}});
        ASSERT_EQ((lava::getMipmapLevelDimension(Dim1D{128}, 2)), image_dimension_t{Dim1D{32}});
        ASSERT_EQ((lava::getMipmapLevelDimension(Dim1D{128}, 0)), image_dimension_t{Dim1D{128}});
    }

    {
        ASSERT_EQ((lava::getMipmapLevelDimension(Dim3D{128, 256, 64}, 7)), (image_dimension_t{Dim3D{1, 2, 1}}));
        ASSERT_EQ((lava::getMipmapLevelDimension(Dim3D{128, 256, 64}, 2)), (image_dimension_t{Dim3D{32, 64, 16}}));
        ASSERT_EQ((lava::getMipmapLevelDimension(Dim3D{128, 256, 64}, 0)), (image_dimension_t{Dim3D{128, 256, 64}}));
    }
}
