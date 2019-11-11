//basicRender.hpp

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <optional>
#include <vector>



class BasicRenderer{
  public: 
struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};
    struct QueueFamilyIndices{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }

    };
    BasicRenderer();
    ~BasicRenderer(); 
    void run();
    bool d_framebufferResized = false;
    VkShaderModule createShaderModule(const std::vector<char>& code);
  private:
    
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
    bool d_enableValidationLayers;
    GLFWwindow* d_window;

    VkInstance d_instance;
    VkDebugUtilsMessengerEXT d_debugMessenger;
    VkSurfaceKHR d_surface;

    VkPhysicalDevice d_physicalDevice = VK_NULL_HANDLE;
    VkDevice d_device;

    VkQueue d_graphicsQueue;
    VkQueue d_presentQueue;

    VkSwapchainKHR d_swapChain;
    std::vector<VkImage> d_swapChainImages;
    VkFormat d_swapChainImageFormat;
    VkExtent2D d_swapChainExtent;
    std::vector<VkImageView> d_swapChainImageViews;
    std::vector<VkFramebuffer> d_swapChainFramebuffers;

    VkRenderPass d_renderPass;
    VkPipelineLayout d_pipelineLayout;
    VkPipeline d_graphicsPipeline;

    VkCommandPool d_commandPool;

    VkBuffer d_vertexBuffer;
    VkDeviceMemory d_vertexBufferMemory;
    VkBuffer d_indexBuffer;
    VkDeviceMemory d_indexBufferMemory;
    std::vector<VkCommandBuffer> d_commandBuffers;

    std::vector<VkSemaphore> d_imageAvailableSemaphores;
    std::vector<VkSemaphore> d_renderFinishedSemaphores;
    std::vector<VkFence> d_inFlightFences;
    std::vector<VkFence> d_imagesInFlight;

    size_t d_currentFrame = 0;

    
    void initVulkan();
      void initWindow();
      void createInstance();
      void setupDebugMessenger(); 
      void createSurface();
      void pickPhysicalDevice();
      void createLogicalDevice();
      void createSwapChain();
      void createImageViews();
      void createRenderPass();
      void createGraphicsPipeline();
      void createFramebuffers();
      void createCommandPool();
      void createVertexBuffer();
      void createIndexBuffer();
      void createCommandBuffers();
      void createSyncObjects();

    void mainLoop();
      void drawFrame();

    void cleanup();
      void cleanupSwapChain();
    
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void recreateSwapChain();
    std::vector<const char*> getRequiredExtensions();
    bool checkValidationLayerSupport();
    bool isDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    

};
