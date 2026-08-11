#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdint>
#include <set>
#include <stdexcept>
#include <vector>

#include <glm/glm.hpp>

inline const uint32_t WIDTH = 800;
inline const uint32_t HEIGHT = 600;
inline const int MAX_FRAMES_IN_FLIGHT = 2;

class VulkanWindow {
public:
    void run();

private:
    GLFWwindow* window = nullptr;

    VkInstance instance = VK_NULL_HANDLE;
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
    VkQueue graphicsQueue = VK_NULL_HANDLE;
    VkQueue presentQueue = VK_NULL_HANDLE;

    VkSwapchainKHR swapChain = VK_NULL_HANDLE;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;

    VkRenderPass renderPass = VK_NULL_HANDLE;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkCommandPool commandPool = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    uint32_t currentFrame = 0;

    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    VkPipeline graphicsPipeline = VK_NULL_HANDLE;
    VkBuffer vertexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE;
    VkBuffer indexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory indexBufferMemory = VK_NULL_HANDLE;

    VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
    VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
    std::vector<VkDescriptorSet> descriptorSets;

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;

    VkImage depthImage = VK_NULL_HANDLE;
    VkDeviceMemory depthImageMemory = VK_NULL_HANDLE;
    VkImageView depthImageView = VK_NULL_HANDLE;

    float elapsedTime = 0.0f;
    float deltaTime = 0.0f;
    float lastFrameTime = 0.0f;

    // Camera
    glm::vec3 cameraPos = glm::vec3(0.0f, -12.0f, 10.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.78f, -0.62f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);
    float yaw = 90.0f;
    float pitch = -40.0f;
    float movementSpeed = 5.0f;
    float mouseSensitivity = 0.1f;
    double lastMouseX = 0.0;
    double lastMouseY = 0.0;
    bool firstMouse = true;

    struct Vertex {
        float pos[3];
        float color[3];
    };

    struct UniformBufferObject {
        glm::mat4 view;
        glm::mat4 proj;
        float time;
    };

    struct QueueFamilyIndices {
        uint32_t graphicsFamily = UINT32_MAX;
        uint32_t presentFamily = UINT32_MAX;

        bool isComplete() const {
            return graphicsFamily != UINT32_MAX && presentFamily != UINT32_MAX;
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities{};
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    void initWindow();
    static void mouseCallbackStatic(GLFWwindow* window, double xpos, double ypos);
    void mouseCallback(double xpos, double ypos);
    void processInput();
    void updateCameraVectors();

    void initVulkan();

    void mainLoop();

    void cleanup();

    void createInstance();

    void createSurface();

    void pickPhysicalDevice();

    bool isDeviceSuitable(VkPhysicalDevice dev);

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice dev);

    bool checkDeviceExtensionSupport(VkPhysicalDevice dev);

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice dev);

    void createLogicalDevice();

    void createSwapChain();

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    void createImageViews();

    void createRenderPass();

    void createGraphicsPipeline();

    VkShaderModule createShaderModule(const uint32_t* code, size_t count);

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

    void createVertexBuffer();

    void createIndexBuffer();

    void createUniformBuffers();

    void createDescriptorSetLayout();

    void createDescriptorPool();

    void createDescriptorSets();

    void createDepthResources();

    void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

    VkFormat findDepthFormat();

    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    bool hasStencilComponent(VkFormat format);

    void updateUniformBuffer(uint32_t currentImage);

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    void createFramebuffers();

    void createCommandPool();

    void createCommandBuffers();

    void createSyncObjects();

    void drawFrame();

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, uint32_t frameIndex);

    void recreateSwapChain();

    void cleanupSwapChain();
};
