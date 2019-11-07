#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<chrono>
#include<vector>
#include<algorithm>
#include<array>
#include <iostream>
#include <stdexcept>
#include <optional>
#include <cstdlib>
#include<cstdint>
#include<fstream>
#include<set>

using std::cout;
using std::endl;
const int WIDTH = 800;
const int HEIGHT = 600;
const int MAX_FRAMES_IN_FLIGHT = 2;
struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;
    static VkVertexInputBindingDescription getBindingDescription(){
        VkVertexInputBindingDescription bindingDescription = {};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescription;
    }
    static std::array<VkVertexInputAttributeDescription,2> getAttributeDescriptions(){
        std::array<VkVertexInputAttributeDescription,2> attributeDescriptions = {};
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex,pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex,color);

        return attributeDescriptions;
    }
};



struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};


//define validation layers
const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

//conditional complication for validation layers
#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif


class basicRenderer {
  public:
    bool shouldRun(){
    return !glfwWindowShouldClose(window_);
    
    }

    void createVertex(float c1,float c2, float c3){
            double x, y;
        if (getCursorPos(x,y)){
            std::cout<<"vertex created"<<endl;
            Vertex vnew;
        glm::vec2 newpos(x,y);
        glm::vec3 newcolor(c1,c2,c3);

        vnew.pos = newpos;
        vnew.color = newcolor;
        verticies.push_back(vnew);
    }
    }

    void addIndice(uint32_t ind){
        indicies.push_back(ind);
      }

    void run() {
        initRenderer();
        mainLoop();
        cleanup();
    }

    void update(){
        createVertexBuffer ();
        createIndexBuffer (); 
    }

    bool getCursorPos(double & x, double & y){
    int state = glfwGetMouseButton(window_,GLFW_MOUSE_BUTTON_LEFT);
    
    if(state==GLFW_PRESS){
      glfwGetCursorPos(window_,&x,&y);
      std::cout<<"x: "<< x << "y: "<<y<<std::endl;
      return true;
    }
    return false;
      
    }



    void initWindow() {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        window_ = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
        glfwSetWindowUserPointer(window_,this);
        glfwSetFramebufferSizeCallback(window_,framebufferResizeCallback);
    }

    void initRenderer() {
        Vertex vert;
        glm::vec2 pos(0.0,0.0);
        glm::vec3 color(0.0,0.0,0.0);
        vert.pos =pos;
        vert.color = color;
        verticies.push_back(vert);
        indicies.push_back(0);
        createInstance();
        setupDebugMessenger();
        createSurface();
        pickPhysicalDevice();
        createLogicalDevice();
        createSwapChain();
        createImageViews();
        createRenderPass();
        createDescriptorSetLayout();
        createGraphicsPipeline();
        createFrameBuffers();
        createCommandPool();
        createVertexBuffer();
        createIndexBuffer();
        createUniformBuffers();
        createDescriptorPool();
        createDescriptorSets();
        createCommandBuffers();
        createSyncObjects();

    }

    //Draw Frame
    void drawFrame(){
        vkWaitForFences(logicalDevice,1,&inFlightFences[currentFrame],VK_TRUE,UINT64_MAX);
        uint32_t imageIndex; 
        VkResult result =  vkAcquireNextImageKHR(logicalDevice,swapChain,UINT64_MAX,
                                         imageAvailableSemaphores[currentFrame],VK_NULL_HANDLE,&imageIndex);
           if(result==VK_ERROR_OUT_OF_DATE_KHR) {
                recreateSwapChain();
                return;
           }else if(result!=VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
              throw std::runtime_error("failed to aquire next image");
           }
        updateUniformBuffer(imageIndex);
        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
        VkPipelineStageFlags waitStages[] ={VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

        VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        vkResetFences(logicalDevice,1,&inFlightFences[currentFrame]);

        if(vkQueueSubmit(graphicsQueue,1,&submitInfo,inFlightFences[currentFrame])!=VK_SUCCESS){
            throw std::runtime_error("failed to submit draw command buffer");
        }

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {swapChain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;

        presentInfo.pResults = nullptr;

        result = vkQueuePresentKHR(presentQueue,&presentInfo);

        if(result==VK_ERROR_OUT_OF_DATE_KHR||result==VK_SUBOPTIMAL_KHR||frameBufferResized) {
            recreateSwapChain();
        }else if(result !=VK_SUCCESS){
            throw std::runtime_error("failed to present swapchain image");
        }


        currentFrame = (currentFrame+1 ) %MAX_FRAMES_IN_FLIGHT;

    }
    void shutdown(){
    cleanup(); 
    }
private:
    std::vector<Vertex> verticies;  
    std::vector<uint32_t> indicies;
//Globalvars
    GLFWwindow* window_;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkQueue presentQueue;
    VkSurfaceKHR surface;
    VkDevice logicalDevice;
    VkQueue graphicsQueue;
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    VkRenderPass renderPass;
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    std::vector<VkFramebuffer> swapChainFrameBuffers;
    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    VkBuffer vertexBuffer;
    VkBuffer indexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkDeviceMemory indexBufferMemory;
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;

    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    bool frameBufferResized = false;
    size_t currentFrame = 0;



    static void framebufferResizeCallback(GLFWwindow* window_, int width, int height){
        auto app = reinterpret_cast<basicRenderer*>(glfwGetWindowUserPointer(window_));
        app->frameBufferResized = true;
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(window_)) {
            glfwPollEvents();
            drawFrame();
        }
        vkDeviceWaitIdle(logicalDevice);
    }

    void cleanup() {
        cleanupSwapChainObjects();

        vkDestroyDescriptorSetLayout(logicalDevice,descriptorSetLayout,nullptr);
        vkDestroyBuffer(logicalDevice,indexBuffer,nullptr);
        vkFreeMemory(logicalDevice,indexBufferMemory,nullptr);

        vkDestroyBuffer(logicalDevice,vertexBuffer,nullptr);
        vkFreeMemory(logicalDevice,vertexBufferMemory,nullptr);
        for(size_t i=0;i<MAX_FRAMES_IN_FLIGHT;i++){
        vkDestroySemaphore(logicalDevice,renderFinishedSemaphores[i],nullptr);
        vkDestroySemaphore(logicalDevice,imageAvailableSemaphores[i],nullptr);
        vkDestroyFence(logicalDevice,inFlightFences[i],nullptr);
        }
        vkDestroyCommandPool(logicalDevice,commandPool,nullptr);

        vkDestroyDevice(logicalDevice,nullptr);

        if(enableValidationLayers){
            DestroyDebugUtilsMessengerEXT(instance,debugMessenger,nullptr);
        }
        vkDestroySurfaceKHR(instance,surface,nullptr);
        vkDestroyInstance(instance, nullptr);
        glfwDestroyWindow(window_);
        glfwTerminate();
    }
    void createDescriptorPool(){
        VkDescriptorPoolSize poolSize = {};
        poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSize.descriptorCount = static_cast<uint32_t>(swapChainImages.size());
        VkDescriptorPoolCreateInfo poolInfo= {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;
        poolInfo.maxSets = static_cast<uint32_t>(swapChainImages.size());

        if(vkCreateDescriptorPool(logicalDevice,&poolInfo,nullptr,&descriptorPool)!=VK_SUCCESS){
            throw std::runtime_error("failed to create descriptor pool");
        }
    }

    void createDescriptorSets(){
        std::vector<VkDescriptorSetLayout> layouts(swapChainImages.size(),descriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo= {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImages.size());
        allocInfo.pSetLayouts = layouts.data();

        descriptorSets.resize(swapChainImages.size());
        if(vkAllocateDescriptorSets(logicalDevice,&allocInfo,descriptorSets.data())!=VK_SUCCESS){
            throw std::runtime_error("failed to allocated descriptor sets");
        }
        for(size_t i=0;i<swapChainImages.size();i++){
            VkDescriptorBufferInfo bufferInfo= {};
            bufferInfo.buffer = uniformBuffers[i];
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferObject);
            
            VkWriteDescriptorSet descriptorWrite= {};
            descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet = descriptorSets[i];
            descriptorWrite.dstArrayElement = 0;
            descriptorWrite.dstBinding = 0;
            descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrite.descriptorCount = 1;
            descriptorWrite.pBufferInfo = &bufferInfo;
            descriptorWrite.pImageInfo = nullptr;
            descriptorWrite.pTexelBufferView = nullptr;

            vkUpdateDescriptorSets(logicalDevice,1,&descriptorWrite,0,nullptr);




        }
        
    }
    void updateUniformBuffer(uint32_t imageIndex){
       static auto startTime = std::chrono::high_resolution_clock::now(); 

       auto currentTime = std::chrono::high_resolution_clock::now();
       float time = std::chrono::duration<float,std::chrono::seconds::period>(currentTime-startTime).count();
        UniformBufferObject ubo = {};
        ubo.model = glm::rotate(glm::mat4(1.0f),time*glm::radians(90.0f),glm::vec3(0.0f,0.0f,1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f,2.0f,2.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,0.0f,1.0f));
        ubo.proj = glm::perspective(glm::radians(45.0f),swapChainExtent.width/(float)swapChainExtent.height
        ,0.1f,10.0f);
        ubo.proj[1][1]*=-1;
        void* data;
        vkMapMemory(logicalDevice,uniformBuffersMemory[imageIndex],0,sizeof(ubo),9,&data);
            memcpy(data,&ubo,sizeof(ubo));
        vkUnmapMemory(logicalDevice,uniformBuffersMemory[imageIndex]);



    }

    void createBuffer(VkDeviceSize size,VkBufferUsageFlags usage,VkMemoryPropertyFlags memProperties,
                                            VkBuffer& buffer,VkDeviceMemory& bufferMemory){
        
        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        if(vkCreateBuffer(logicalDevice,&bufferInfo,nullptr,&buffer)!=VK_SUCCESS){
            throw std::runtime_error("failed to create vertex Buffer");
        }
        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(logicalDevice,buffer,&memRequirements);

        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits,memProperties);
        
        if(vkAllocateMemory(logicalDevice,&allocInfo,nullptr,&bufferMemory)!=VK_SUCCESS){
            throw std::runtime_error("failed to allocate vertex buffer memeory");
        }

        vkBindBufferMemory(logicalDevice,buffer,bufferMemory,0);


    }

    void createDescriptorSetLayout(){
        VkDescriptorSetLayoutBinding uboLayoutBinding = {};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorCount= 1;
        uboLayoutBinding.descriptorType= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        VkDescriptorSetLayoutCreateInfo layoutInfo = {};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount= 1;
        layoutInfo.pBindings = &uboLayoutBinding;
        if(vkCreateDescriptorSetLayout(logicalDevice,&layoutInfo,nullptr,&descriptorSetLayout)!=VK_SUCCESS){
                throw std::runtime_error("failed to create descriptor set layout");
        }
    }

    void createUniformBuffers(){
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);

        uniformBuffers.resize(swapChainImages.size());
        uniformBuffersMemory.resize(swapChainImages.size());

        for(size_t i =0;i<swapChainImages.size();i++){
            createBuffer(bufferSize,VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
            |VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,uniformBuffers[i],uniformBuffersMemory[i]);
        }

    }
    void createIndexBuffer(){

        VkDeviceSize bufferSize  = indicies.size()*sizeof(indicies[0]);

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        
        createBuffer(bufferSize,VK_BUFFER_USAGE_TRANSFER_SRC_BIT,VK_MEMORY_PROPERTY_HOST_COHERENT_BIT|
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,stagingBuffer,stagingBufferMemory);

        void* data;
        vkMapMemory(logicalDevice, stagingBufferMemory,0,bufferSize,0,&data);
            memcpy(data,indicies.data(),(size_t)bufferSize);
        vkUnmapMemory(logicalDevice,stagingBufferMemory);


        createBuffer(bufferSize,VK_BUFFER_USAGE_TRANSFER_DST_BIT|VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,indexBuffer,indexBufferMemory);

        copyBuffer(stagingBuffer,indexBuffer,bufferSize);

        vkDestroyBuffer(logicalDevice,stagingBuffer,nullptr);
        vkFreeMemory(logicalDevice,stagingBufferMemory,nullptr);
    }

    void createVertexBuffer(){
        VkDeviceSize bufferSize = verticies.size()*sizeof(verticies[0]);


        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;



        createBuffer(bufferSize,VK_BUFFER_USAGE_TRANSFER_SRC_BIT,VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
        |VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,stagingBuffer,stagingBufferMemory);

        void* data;
        vkMapMemory(logicalDevice,stagingBufferMemory,0,bufferSize,0,&data);
            memcpy(data,verticies.data(),(size_t) bufferSize);
        vkUnmapMemory(logicalDevice,stagingBufferMemory);

        createBuffer(bufferSize,VK_BUFFER_USAGE_TRANSFER_DST_BIT|VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,vertexBuffer,vertexBufferMemory);
        copyBuffer(stagingBuffer,vertexBuffer,bufferSize);
        vkDestroyBuffer(logicalDevice,stagingBuffer,nullptr);
        vkFreeMemory(logicalDevice,stagingBufferMemory,nullptr);
    }

    void copyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size){
        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = commandPool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(logicalDevice,&allocInfo,&commandBuffer);

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer,&beginInfo);

        VkBufferCopy copyRegion = {};
        copyRegion.dstOffset = 0;
        copyRegion.srcOffset = 0;
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer,src,dst,1,&copyRegion);

        vkEndCommandBuffer(commandBuffer);
        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(graphicsQueue,1,&submitInfo,nullptr);
        vkDeviceWaitIdle(logicalDevice);
        vkFreeCommandBuffers(logicalDevice,commandPool,1,&commandBuffer);
    }

    uint32_t findMemoryType(uint32_t typeFilter,VkMemoryPropertyFlags properties){
            VkPhysicalDeviceMemoryProperties memProperties;
            vkGetPhysicalDeviceMemoryProperties(physicalDevice,&memProperties);
            for(uint32_t i = 0 ; i<memProperties.memoryTypeCount;i++){
                //we need to find the index of the memory type that has the right type, and satisfies
                //the indicated properties
                if( (typeFilter&(1<<i)) &&
                     ((memProperties.memoryTypes[i].propertyFlags&properties)==properties)
                     ){
                    return i;
                }

            }
            throw std::runtime_error("failed to find suitable memory type");

    }

    void recreateSwapChain(){
        int width=0,height=0;
        while(width==0||height==0){
            glfwGetFramebufferSize(window_,&width,&height);
            glfwWaitEvents();
        }
        vkDeviceWaitIdle(logicalDevice);

        cleanupSwapChainObjects();
        
        createSwapChain();
        createUniformBuffers();
        createDescriptorPool();
        createDescriptorSets();
        createImageViews();
        createRenderPass();
        createGraphicsPipeline();
        createFrameBuffers();
        createCommandBuffers();


    }

    void cleanupSwapChainObjects(){

        for(auto framebuffer : swapChainFrameBuffers){
            vkDestroyFramebuffer(logicalDevice,framebuffer,nullptr);
        }

        vkFreeCommandBuffers(logicalDevice,commandPool,static_cast<uint32_t>(commandBuffers.size()),
        commandBuffers.data());

        vkDestroyPipeline(logicalDevice,graphicsPipeline,nullptr);
        vkDestroyPipelineLayout(logicalDevice,pipelineLayout,nullptr);
        vkDestroyRenderPass(logicalDevice,renderPass,nullptr);
        for(auto imageView: swapChainImageViews){
            vkDestroyImageView(logicalDevice,imageView,nullptr);
        }
        vkDestroySwapchainKHR(logicalDevice,swapChain,nullptr);
        for(size_t i=0;i<swapChainImages.size();i++) {
            vkDestroyBuffer(logicalDevice,uniformBuffers[i],nullptr);
            vkFreeMemory(logicalDevice,uniformBuffersMemory[i],nullptr);
        }
        vkDestroyDescriptorPool(logicalDevice,descriptorPool,nullptr);
    }
    //create Semaphores
    void createSyncObjects(){
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        VkSemaphoreCreateInfo semaphoreInfo = {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        VkFenceCreateInfo fenceCreateInfo = {};
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for(size_t i=0;i< MAX_FRAMES_IN_FLIGHT;i++){
            if( vkCreateSemaphore(logicalDevice,&semaphoreInfo,nullptr,&imageAvailableSemaphores[i])
            !=VK_SUCCESS 
            || vkCreateSemaphore(logicalDevice,&semaphoreInfo,nullptr,&renderFinishedSemaphores[i])
            !=VK_SUCCESS
            || vkCreateFence(logicalDevice,&fenceCreateInfo,nullptr,&inFlightFences[i])!=VK_SUCCESS){
                
                throw std::runtime_error("unable to create syncronization objects");
            }
        }        

    }
    //set up commandBuffers
    void createCommandBuffers(){
        commandBuffers.resize(swapChainFrameBuffers.size());
        VkCommandBufferAllocateInfo allocateInfo = {};
        allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocateInfo.commandPool = commandPool;
        allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocateInfo.commandBufferCount = (uint32_t) commandBuffers.size();
        if(vkAllocateCommandBuffers(logicalDevice,&allocateInfo,commandBuffers.data())!=VK_SUCCESS){
            throw std::runtime_error("failed to create Command Buffers");
        }
        for(size_t i=0;i<commandBuffers.size(); i++){
            VkCommandBufferBeginInfo beginInfo = {};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = 0;
            beginInfo.pInheritanceInfo = nullptr;

            if(vkBeginCommandBuffer(commandBuffers[i],&beginInfo)!=VK_SUCCESS){
                throw std::runtime_error("failed to begin recording command buffer");
            }
        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass;
        renderPassInfo.framebuffer = swapChainFrameBuffers[i];
        renderPassInfo.renderArea.offset = {0,0};
        renderPassInfo.renderArea.extent = swapChainExtent;

        VkClearValue clearValue = {0.0f,0.0f,0.0f,1.0f};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearValue;

        vkCmdBeginRenderPass(commandBuffers[i],&renderPassInfo,VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(commandBuffers[i],VK_PIPELINE_BIND_POINT_GRAPHICS,graphicsPipeline);

        VkBuffer vertexBuffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffers[i],0,1,vertexBuffers,offsets);

        vkCmdBindIndexBuffer(commandBuffers[i],indexBuffer,0,VK_INDEX_TYPE_UINT16);

        vkCmdBindDescriptorSets(commandBuffers[i],VK_PIPELINE_BIND_POINT_GRAPHICS,pipelineLayout,
        0,1,&descriptorSets[i],0,nullptr);

        vkCmdDrawIndexed(commandBuffers[i],static_cast<uint32_t>(indicies.size()),1,0,0,0);
        vkCmdEndRenderPass(commandBuffers[i]);

        if(vkEndCommandBuffer(commandBuffers[i])!=VK_SUCCESS){
            throw std::runtime_error("failed to end command buffer recording");
        }


        
        }


    }
    //set up commandPool
    void createCommandPool(){
        QueueFamilyIndicies queueFamilyIndicies = findQueueFamily(physicalDevice);
       VkCommandPoolCreateInfo poolInfo = {} ;
       poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
       poolInfo.queueFamilyIndex = queueFamilyIndicies.graphicsFamily.value();
       poolInfo.flags = 0;
       if(vkCreateCommandPool(logicalDevice,&poolInfo,nullptr,&commandPool)!=VK_SUCCESS){
           throw std::runtime_error("failed to create command Pool");

       }
       
       
    }
    //set up frameBuffers
    void createFrameBuffers(){

        swapChainFrameBuffers.resize(swapChainImageViews.size());

        for(size_t i =0 ; i<swapChainImageViews.size();i++){
            VkImageView attachments[] = {
                swapChainImageViews[i]
            };
            VkFramebufferCreateInfo framebufferInfo = {};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = swapChainExtent.width;
            framebufferInfo.height = swapChainExtent.height;
            framebufferInfo.layers = 1;

            if(vkCreateFramebuffer(logicalDevice,&framebufferInfo,nullptr,&swapChainFrameBuffers[i])!=VK_SUCCESS){
            
                throw std::runtime_error("failed to create frameBuffer");
            }
        }

    }
    //set up Render passes
    void createRenderPass(){
        VkAttachmentDescription colorAttachment = {};
        colorAttachment.format = swapChainImageFormat;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef = {};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

        subpass.colorAttachmentCount= 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        
        VkSubpassDependency dependency = {};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        
        
        VkRenderPassCreateInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;
        
        if(vkCreateRenderPass(logicalDevice,&renderPassInfo,nullptr,&renderPass)!=VK_SUCCESS){
            throw std::runtime_error("failed to create RenderPass");
        }

        
        

    }
    //set up graphics pipeline
    static std::vector<char> readFile(const std::string& filename){
        std::string filePath = "/Users/willchambers/Projects/VulkanPractice/";
        std::ifstream file(filePath+filename,std::ios::ate | std::ios::binary);

        if(!file.is_open()){
            throw std::runtime_error("failed to open file! at "+filePath+filename);
        }
        size_t filesize = file.tellg();
        std::vector<char> buffer(filesize);

        file.seekg(0);

        file.read(buffer.data(),filesize);

        return buffer;
    }

    VkShaderModule createShaderModule(const std::vector<char> &shaderData){
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = shaderData.size();
        createInfo.pCode = reinterpret_cast<const uint32_t *>(shaderData.data());

        VkShaderModule shaderModule;

        if(vkCreateShaderModule(logicalDevice,&createInfo,nullptr,&shaderModule)!=VK_SUCCESS){
            throw std::runtime_error("shader module creation failed");
        }
        return shaderModule;
    }

   void createGraphicsPipeline(){
       auto vertShaderCode = readFile("shaders/vert.spv");
       auto fragShaderCode = readFile("shaders/frag.spv");
    VkShaderModule vertModule = createShaderModule(vertShaderCode);
    VkShaderModule fragModule = createShaderModule(fragShaderCode);
    //do shader processing here..
    VkPipelineShaderStageCreateInfo vertShaderStageCreateInfo = {};

    vertShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageCreateInfo.module = vertModule;
    vertShaderStageCreateInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageCreateInfo = {};

    fragShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageCreateInfo.module = fragModule;
    fragShaderStageCreateInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[]= {vertShaderStageCreateInfo,fragShaderStageCreateInfo};

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(
                                        attributeDescriptions.size());

    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewPort = {};
    viewPort.x = 0.0f;
    viewPort.y = 0.0f;
    viewPort.width = (float) swapChainExtent.width;
    viewPort.height = (float) swapChainExtent.height;
    viewPort.minDepth =0.0f;
    viewPort.maxDepth = 1.0f;

    VkRect2D scissor ={};
    scissor.offset = {0,0};
    scissor.extent = swapChainExtent;

    VkPipelineViewportStateCreateInfo viewPortState = {};
    viewPortState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewPortState.viewportCount = 1;
    viewPortState.pViewports = &viewPort;
    viewPortState.scissorCount = 1;
    viewPortState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasClamp = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f;
    rasterizer.depthBiasClamp = 0.0f;
    rasterizer.depthBiasSlopeFactor = 0.0f;
    
    VkPipelineMultisampleStateCreateInfo multiSampling = {};
    multiSampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multiSampling.sampleShadingEnable = VK_FALSE;
    multiSampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multiSampling.minSampleShading = 1.0f;
    multiSampling.pSampleMask = nullptr;
    multiSampling.alphaToCoverageEnable = VK_FALSE;
    multiSampling.alphaToOneEnable = VK_FALSE;


    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_A_BIT| VK_COLOR_COMPONENT_B_BIT 
                                    | VK_COLOR_COMPONENT_G_BIT|VK_COLOR_COMPONENT_R_BIT;
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
    
    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    VkDynamicState dynamicStates[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_LINE_WIDTH
    };

    VkPipelineDynamicStateCreateInfo dynamicState = {};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = 2;
    dynamicState.pDynamicStates = dynamicStates;
    
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    if(vkCreatePipelineLayout(logicalDevice,&pipelineLayoutInfo,nullptr,&pipelineLayout)!=VK_SUCCESS){
        throw std::runtime_error("failed to create pipeline Layout");
    }

    

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType  = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;

    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewPortState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multiSampling;
    pipelineInfo.pDepthStencilState = nullptr;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = nullptr;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;
    
    if(vkCreateGraphicsPipelines(logicalDevice,VK_NULL_HANDLE,1,&pipelineInfo,nullptr,&graphicsPipeline)!=VK_SUCCESS){
        throw std::runtime_error("failed to create graphics Pipeline");
    }

    vkDestroyShaderModule(logicalDevice,vertModule,nullptr);
    vkDestroyShaderModule(logicalDevice,fragModule,nullptr);
   }
    //set up image Views
   void createImageViews(){
        swapChainImageViews.resize(swapChainImages.size());
        for(size_t i=0;i<swapChainImageViews.size();i++){
            VkImageViewCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = swapChainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = swapChainImageFormat;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;  
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;  
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;  
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;  
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;
            if(vkCreateImageView(logicalDevice,&createInfo,nullptr,&swapChainImageViews[i])!=VK_SUCCESS) {
                throw std::runtime_error("unable to create image views ");
            }
            
        }

   }
   //set up SwapChain 
   void createSwapChain(){
       SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);

       VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
       VkPresentModeKHR presentMode = chooseSwapSurfacePresentMode(swapChainSupport.presentModes);
       VkExtent2D swapExtent = chooseSwapExtent(swapChainSupport.capabilities);
       uint32_t minImageCount = swapChainSupport.capabilities.minImageCount+1;
       if(swapChainSupport.capabilities.maxImageCount>0 && minImageCount>swapChainSupport.capabilities.maxImageCount){
           minImageCount = swapChainSupport.capabilities.maxImageCount;
       }
        VkSwapchainCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surface;
        createInfo.minImageCount = minImageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageArrayLayers = 1;
        createInfo.imageExtent = swapExtent;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        QueueFamilyIndicies indices = findQueueFamily(physicalDevice);
        uint32_t queueFamilyIndicies[]  = {indices.graphicsFamily.value(),indices.presentFamily.value()};
        if(indices.presentFamily!=indices.graphicsFamily){
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndicies;

        } else{
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }
        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;
    

    if(vkCreateSwapchainKHR(logicalDevice,&createInfo,nullptr,&swapChain)!=VK_SUCCESS){
        throw std::runtime_error("swapChain creation Failed");
    }
    uint32_t imageCount;
    vkGetSwapchainImagesKHR(logicalDevice,swapChain,&imageCount,nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(logicalDevice,swapChain,&imageCount,swapChainImages.data());

    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = swapExtent;

   }
    struct SwapChainSupportDetails{
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
    };
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities){
        if(capabilities.currentExtent.width!=UINT32_MAX){
            return capabilities.currentExtent;
        }
        else{
            int width,height;
            glfwGetFramebufferSize(window_,&width,&height);

            VkExtent2D actualExtent = {
               static_cast<uint32_t> (width) ,
                
               static_cast<uint32_t> (height) 
                };

            actualExtent.width = std::max(capabilities.minImageExtent.width,std::min(capabilities.maxImageExtent.width
            ,capabilities.currentExtent.width));
            actualExtent.height = std::max(capabilities.minImageExtent.height,std::min(capabilities.maxImageExtent.height
            ,capabilities.currentExtent.height));

            return actualExtent;

        }
    }
    //iterate through the provided present modes vector, and return a prefered one if you can, if not return 
    //one that we are garunteed to have
    VkPresentModeKHR chooseSwapSurfacePresentMode(const std::vector<VkPresentModeKHR> availablePresentModes){
        for(const auto& presentMode: availablePresentModes){
            if(presentMode == VK_PRESENT_MODE_MAILBOX_KHR){
                return presentMode;
            }
        }
        return VK_PRESENT_MODE_FIFO_KHR;

    }
    //look through the available formats vector provided, and return a prefered one, if possible,
    //if not, then just return the firstone
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR> &availableFormats){
       for(const auto& format: availableFormats) {
           if(format.format==VK_FORMAT_B8G8R8A8_UNORM &&format.colorSpace==VK_COLOR_SPACE_SRGB_NONLINEAR_KHR){
               //this is a good color format and space, so if we can get it, take it
               return format;
           }
       }
        // if, for whatever reason we can't get ahold of the nice one, just use this one instead
       return availableFormats[0];
    }
    //query the physical device for swap chain support, populating a swap chain support details struct
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device){
        SwapChainSupportDetails details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device,surface,&details.capabilities);
        uint32_t formatCount; 
        vkGetPhysicalDeviceSurfaceFormatsKHR(device,surface,&formatCount,nullptr);
        if(formatCount!=0){
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device,surface,&formatCount,details.formats.data());
        }
        uint32_t presentModesCount; 
        vkGetPhysicalDeviceSurfacePresentModesKHR(device,surface,&presentModesCount,nullptr);
        if(presentModesCount !=0){

            details.presentModes.resize(presentModesCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device,surface,&presentModesCount,details.presentModes.data());
        }
        return details;
    }
// ensure that all required extensions for the swap chain are supported
    bool checkDeviceExtensionSupport(VkPhysicalDevice device){
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device,nullptr,&extensionCount,nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);

        vkEnumerateDeviceExtensionProperties(device,nullptr,&extensionCount,availableExtensions.data());
        std::set<std::string> requiredExtensions(deviceExtensions.begin(),deviceExtensions.end());


        for(const auto &extension : availableExtensions){

            requiredExtensions.erase(extension.extensionName);

        }
        //if we've emptied the required extensions list, then required extensions are a subset of the extensions 
        // that we have, and we're in business! 
        return requiredExtensions.empty();
    }

    //Surface set Up:
    void createSurface(){
        if(glfwCreateWindowSurface(instance,window_,nullptr,&surface)!=VK_SUCCESS)
        throw std::runtime_error("Surface creation failed, re-think your life");

    }
    //Logical Device Set Up:
    void createLogicalDevice(){
        QueueFamilyIndicies indicies = findQueueFamily(physicalDevice);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {indicies.graphicsFamily.value(),indicies.presentFamily.value()};
        
        float QueuePriority = 1.0f;
        
        for(uint32_t queueFamilyIndex:uniqueQueueFamilies){
            VkDeviceQueueCreateInfo queueCreateInfo = {};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &QueuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }
        
        VkPhysicalDeviceFeatures deviceFeatures = {};

        VkDeviceCreateInfo deviceCreateInfo = {};
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
        
        deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
        deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

        if(enableValidationLayers){
            deviceCreateInfo.enabledLayerCount = static_cast<uint32_t> (validationLayers.size());
            deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
        }else{
            deviceCreateInfo.enabledLayerCount = 0;
        }
        
        if(vkCreateDevice(physicalDevice,&deviceCreateInfo,nullptr,&logicalDevice)!=VK_SUCCESS){
            throw std::runtime_error("failed to create logical Device");
        }
        //the zero below referse to the fact that we have only one queue in the array
        vkGetDeviceQueue(logicalDevice,indicies.graphicsFamily.value(),0,&graphicsQueue);
        vkGetDeviceQueue(logicalDevice,indicies.presentFamily.value(),0,&presentQueue);
    }

    // PHYSICAL DEVICE SET UP: 
    void pickPhysicalDevice(){
        uint32_t physicalDeviceCount =0;
        vkEnumeratePhysicalDevices(instance,&physicalDeviceCount,nullptr);

        if(physicalDeviceCount ==0) throw std::runtime_error("no Vulkan capable graphics cards found, Disaster!");

        std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);

        vkEnumeratePhysicalDevices(instance, &physicalDeviceCount,physicalDevices.data());

        for(VkPhysicalDevice device: physicalDevices){
            if(deviceIsSuitable(device)){
                physicalDevice = device;
                break;
            }
        }
        if(physicalDevice == VK_NULL_HANDLE){
            throw std::runtime_error("No vulkan physical devices with required properties found");
        }
    }

    bool deviceIsSuitable(VkPhysicalDevice device){
            QueueFamilyIndicies indicies = findQueueFamily(device);

            bool extensionsSupported = checkDeviceExtensionSupport(device);
            bool swapChainAdequate = false;
           SwapChainSupportDetails swapChainDetails = querySwapChainSupport(device);
            if(extensionsSupported){
                swapChainAdequate = !swapChainDetails.formats.empty()&&!swapChainDetails.presentModes.empty();
            }
            return indicies.isComplete()&&extensionsSupported&&swapChainAdequate;
    }

    struct QueueFamilyIndicies {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;
        
        bool isComplete(){
            return graphicsFamily.has_value()&&presentFamily.has_value();
        }
    };

    QueueFamilyIndicies findQueueFamily(VkPhysicalDevice device){
        QueueFamilyIndicies indicies;
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device,&queueFamilyCount,nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device,&queueFamilyCount,queueFamilies.data());
        int i=0;
        for(const auto &queueFamily :queueFamilies){
           if(queueFamily.queueCount>0&&queueFamily.queueFlags&VK_QUEUE_GRAPHICS_BIT){
               indicies.graphicsFamily = i;
           }
           VkBool32 presentSupport = false;
           vkGetPhysicalDeviceSurfaceSupportKHR(device,i,surface,&presentSupport);
           if(queueFamily.queueCount>0&&presentSupport){
                indicies.presentFamily = i;
           }

           i++;
        }
        return indicies;
    }
    //Vulkan Initiazation methods
    void createInstance(){
            if(enableValidationLayers&&!checkValidationLayersSupport()){
                throw std::runtime_error("validation layers requested, but unavailable :<");
            }
        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1,0,0);
        appInfo.apiVersion = VK_API_VERSION_1_0;
        
        VkInstanceCreateInfo createInfo  = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        //enumerate extensions for vkInstance

        auto extensions = getRequiredExtensions();        
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
        debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        if(enableValidationLayers){
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
            populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
        }else{
        createInfo.enabledLayerCount = 0; 
        createInfo.pNext = nullptr;
        }

          validateAndPrintExtensions(); 
        if(vkCreateInstance(&createInfo,nullptr,&instance) != VK_SUCCESS){
            throw std::runtime_error("instance creation failed");
        }

    }
    
    std::vector<const char*> getRequiredExtensions(){
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions; 
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions,glfwExtensions+glfwExtensionCount); //wow

        if(enableValidationLayers){
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

        //make sure that all glfw extensions are supported, otherwise throw a runtime error
    void validateAndPrintExtensions(){
        uint32_t glfwExtensionCount = 0; 
        const char** glfwExtensions; 
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

         uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr,&extensionCount,extensions.data());

        for(uint32_t i=0; i<glfwExtensionCount;i++){
             bool found = false; 
             for(VkExtensionProperties prop: extensions){
                 if(i==0) std::cout<<prop.extensionName<<std::endl;
                 if(strcmp(prop.extensionName,glfwExtensions[i])==0) found = true;
            }
            if(!found){
                throw std::runtime_error("required extension for glfw not supported");
            }
            found = false;
        }

    }

    bool checkValidationLayersSupport(){
        uint32_t layerCount; 
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers (layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount,availableLayers.data());

        for(const char* requestedLayer : validationLayers){
            bool layerFound; 

            for(VkLayerProperties availableLayer: availableLayers){
                if(strcmp(requestedLayer,availableLayer.layerName)==0) layerFound = true; 
            }
            if(!layerFound) return false;
            layerFound = false; 
        }
     return true;
    }
// Define methods for Message CallBacks
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

    

    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
             const VkAllocationCallbacks * pAllocator , VkDebugUtilsMessengerEXT* pDebugMessenger){
                 auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
                 if( func!=nullptr){
                     return func(instance,pCreateInfo,pAllocator,pDebugMessenger);
                 }else{
                     return VK_ERROR_EXTENSION_NOT_PRESENT;
                 }
             }
             
    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo){
   createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
       createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT|VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
       createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
       createInfo.pfnUserCallback = debugCallback;
       createInfo.flags = 0;
       createInfo.pUserData = nullptr; 
}

    void setupDebugMessenger(){
        if(!enableValidationLayers) return;
       VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
      populateDebugMessengerCreateInfo(createInfo);
      if(CreateDebugUtilsMessengerEXT(instance, &createInfo,nullptr,&debugMessenger)!=VK_SUCCESS) {
          throw std::runtime_error("Messenger setup failed, RIP");
      }
    }

};



using std::cout;
using std::endl;
int main(){
  basicRenderer renderer;
    cout<<"renderer instantiated"<<endl;
  renderer.initWindow();
    cout<<"window created"<<endl;
  //renderer.initRenderer();
    cout<<"renderer initialized"<<endl;
 while(renderer.shouldRun()) {
      
 }


}
