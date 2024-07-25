#version 430 core

// 将rgb565分离为8bit rgb数据的计算着色器

// 分配本地工作组大小。图像尺寸一般是16的整数倍。
layout (local_size_x = 16, local_size_y = 16) in;

layout (r16ui) uniform uimageBuffer rawData;
layout (r8ui) uniform uimageBuffer rData;
layout (r8ui) uniform uimageBuffer gData;
layout (r8ui) uniform uimageBuffer bData;

layout (r32i) uniform int width;
layout (r32i) uniform int height;

// uvec3 gl_WorkGroupSize 本地工作组大小，存储的就是local_size_x/y/z
// uvec3 gl_NumWorkGroups 传递给glDispatchCompute()的参数，可以知道全局工作组的大小
// uvec3 gl_LocalInvocationID 当前单元在本地工作组中的位置
// uvec3 gl_WorkGroupID 当前本地工作组在全局工作组中的位置
// uvec3 gl_GlobalInvocationID 当前单元在全局工作组中的位置
// uint gl_LocalInvocationIndex 当前单元在本地工作组中的一维索引

void main(void) {
    int R_MASK = 0x00F8;
int G_MASK = 0x00FC;
int B_MASK = 0x00F8;

int R_SHIFT = 8;
int G_SHIFT = 3;
int B_SHIFT = 3;

    int localGroupIndex = (int)(gl_WorkGroupID.y * gl_NumWorkGroups.x + gl_WorkGroupID.x);
    int unitIndex = (int)(localGroupIndex * gl_WorkGroupSize.x * gl_WorkGroupSize.y + gl_LocalInvocationIndex);

    uint16_t t = imageLoad(rawData, unitIndex);
    uint8_t r = (uint8_t)((t >> R_SHIFT) & R_MASK);
    uint8_t g = (uint8_t)((t >> G_SHIFT) & G_MASK);
    uint8_t b = (uint8_t)((t << B_SHIFT) & B_MASK);

    imageStore(rData, unitIndex, r);
    imageStore(gData, unitIndex, g);
    imageStore(bData, unitIndex, b);
}
