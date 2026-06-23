#include "../vulkWindow.h"
#include "vulkanBootstrap.h"
#include <iostream>

namespace vulkanBootstrap {
	void buildInstance() {
		vkb::InstanceBuilder vulkInstanceBuilder;
		auto instance_ret = vulkInstanceBuilder
			.set_app_name("vulk-engine")
			.request_validation_layers()
			.use_default_debug_messenger()
			.build();

		if (!instance_ret) {
			std::cerr << "We have a problem on Instance: " << instance_ret.error().message() << std::endl;
			return;
		}

		vkbInstance = instance_ret.value();
		vulkInstance = vkbInstance.instance;
	}

	void selectDevice() {
		vkb::PhysicalDeviceSelector selector{ vulkanBootstrap::getVKBinstance()};
		auto phys_device_ret = selector
			.set_surface(vulkanWindow::getSurface())
			.set_minimum_version(1, 3) 
			.select();

		if (!phys_device_ret) {
			std::cerr << "We have a problem for selecting the GPU: " << phys_device_ret.error().message() << std::endl;
			return;
		}

		vkb::PhysicalDevice vulkPhysicalDevice = phys_device_ret.value();

		vkb::DeviceBuilder device_builder{ vulkPhysicalDevice };
		auto device_ret = device_builder.build();

		if (!device_ret) {
			std::cerr << "We have a problem for creating the logical device: " << device_ret.error().message() << std::endl;
			return;
		}
		vkbDevice = device_ret.value();
		vulkDevice = vkbDevice.device;
	}

	void Destroy() {
		vkb::destroy_device(vkbDevice);
		vkb::destroy_instance(vkbInstance);
	}

	VkInstance getInstance() {
		return vulkInstance;
	}

	vkb::Instance getVKBinstance() {
		return vkbInstance;
	}

	VkDevice getDevice() {
		return vulkDevice;
	}
}