#include <gtest/gtest.h>
#include <Lava/vk-sys/Device.h>

TEST(test_device, features) {
    {
        lava::Instance instance = lava::InstanceBuilder(lava::VulkanVersion::VERSION_1_0) //
                                      .build();

        lava::Device device = lava::DeviceBuilder{instance} //
                                  .withGeometryShader()
                                  .build();
        ASSERT_TRUE(device.features->geometryShader);
    }
    {
        lava::Instance instance = lava::InstanceBuilder(lava::VulkanVersion::VERSION_1_0) //
                                      .build();

        lava::Device device = lava::DeviceBuilder{instance} //
                                  .build();
        ASSERT_FALSE(device.features->geometryShader);
    }
}
