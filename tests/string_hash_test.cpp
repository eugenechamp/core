#include<core/string_hash.hpp>
#include<core/hashmap.hpp>
#include <gtest/gtest.h>

static constexpr StringView names[100] = {
  "Transform", "Position", "Rotation", "Scale", "Velocity",
  "Acceleration", "Mass", "Force", "Gravity", "Friction",
  "Camera", "Projection", "View", "Viewport", "RenderTarget",
  "Shader", "Material", "Texture", "Mesh", "VertexBuffer",
  "IndexBuffer", "Pipeline", "RenderPass", "FrameBuffer", "Swapchain",
  "Light", "DirectionalLight", "PointLight", "SpotLight", "Shadow",
  "Animation", "Animator", "Skeleton", "Bone", "Keyframe",
  "Timeline", "StateMachine", "Transition", "BlendTree", "Clip",
  "AudioSource", "AudioListener", "Sound", "Music", "Volume",
  "Pitch", "Loop", "SpatialAudio", "Mixer", "Channel",
  "Input", "Keyboard", "Mouse", "Gamepad", "Button",
  "Axis", "Action", "Binding", "Cursor", "Controller",
  "Entity", "Component", "System", "World", "Scene",
  "Hierarchy", "Parent", "Child", "Tag", "Layer",
  "Physics", "Collider", "BoxCollider", "SphereCollider", "CapsuleCollider",
  "RigidBody", "Trigger", "Constraint", "Joint", "Contact",
  "Network", "Client", "Server", "Packet", "Connection",
  "Session", "Replication", "Serialization", "Snapshot", "Latency",
  "Resource", "Asset", "Loader", "Cache", "Manager", "Vertex", "Simulate", 
  "Name", "Event", "UnknownType",
};

static_assert(StringHash() == StringHash(""));

bool find_string(StringHash sh) {
  bool found = false;
  for(auto& name : names) 
  {
    if(sh == StringHash(name))
    {
      found = true;
      break;
    }
  }
  return found;
}

TEST(StringHash, Compare) {
  EXPECT_TRUE(StringHash(names[0]) == StringHash(names[0]));
  EXPECT_TRUE(StringHash(names[0]) != StringHash(names[1]));
}

TEST(StringHash, FindString) {
  EXPECT_TRUE(find_string(StringHash("SpatialAudio")));
  EXPECT_FALSE(find_string(StringHash("Spa tialAubio")));
  EXPECT_FALSE(find_string(StringHash("This string will not be found")));

  for(auto& name : names) 
  {
    EXPECT_TRUE(find_string(StringHash(name)));
  }
}

TEST(StringHash, SmallSwitch) {
  using namespace core::sh_literals;
  StringHash string_hash("Component");
  switch(string_hash) {
    case "Transform"_sh64: { FAIL(); } break;
    case "Music"_sh64: { FAIL(); } break;
    case "Audio"_sh64: { FAIL(); } break;
    case "Component"_sh64: { SUCCEED(); } break;
    case "RenderPass"_sh64: { FAIL(); } break;
    case "Cursor"_sh64: { FAIL(); } break;
    case "World"_sh64: { FAIL(); } break;
    case "DirectionalLight"_sh64: { FAIL(); } break;
    default:
      FAIL();
  };
}

TEST(StringHash, BigSwitch) {
  srand(u32(std::time(nullptr)));
  for(u32 i{}; i < 1000; i++)
  {
    StringHash string_hash(names[rand() % 100]);
    switch (string_hash) {
      case StringHash(names[0]): break;
      case StringHash(names[1]): break;
      case StringHash(names[2]): break;
      case StringHash(names[3]): break;
      case StringHash(names[4]): break;
      case StringHash(names[5]): break;
      case StringHash(names[6]): break;
      case StringHash(names[7]): break;
      case StringHash(names[8]): break;
      case StringHash(names[9]): break;
      case StringHash(names[10]): break;
      case StringHash(names[11]): break;
      case StringHash(names[12]): break;
      case StringHash(names[13]): break;
      case StringHash(names[14]): break;
      case StringHash(names[15]): break;
      case StringHash(names[16]): break;
      case StringHash(names[17]): break;
      case StringHash(names[18]): break;
      case StringHash(names[19]): break;
      case StringHash(names[20]): break;
      case StringHash(names[21]): break;
      case StringHash(names[22]): break;
      case StringHash(names[23]): break;
      case StringHash(names[24]): break;
      case StringHash(names[25]): break;
      case StringHash(names[26]): break;
      case StringHash(names[27]): break;
      case StringHash(names[28]): break;
      case StringHash(names[29]): break;
      case StringHash(names[30]): break;
      case StringHash(names[31]): break;
      case StringHash(names[32]): break;
      case StringHash(names[33]): break;
      case StringHash(names[34]): break;
      case StringHash(names[35]): break;
      case StringHash(names[36]): break;
      case StringHash(names[37]): break;
      case StringHash(names[38]): break;
      case StringHash(names[39]): break;
      case StringHash(names[40]): break;
      case StringHash(names[41]): break;
      case StringHash(names[42]): break;
      case StringHash(names[43]): break;
      case StringHash(names[44]): break;
      case StringHash(names[45]): break;
      case StringHash(names[46]): break;
      case StringHash(names[47]): break;
      case StringHash(names[48]): break;
      case StringHash(names[49]): break;
      case StringHash(names[50]): break;
      case StringHash(names[51]): break;
      case StringHash(names[52]): break;
      case StringHash(names[53]): break;
      case StringHash(names[54]): break;
      case StringHash(names[55]): break;
      case StringHash(names[56]): break;
      case StringHash(names[57]): break;
      case StringHash(names[58]): break;
      case StringHash(names[59]): break;
      case StringHash(names[60]): break;
      case StringHash(names[61]): break;
      case StringHash(names[62]): break;
      case StringHash(names[63]): break;
      case StringHash(names[64]): break;
      case StringHash(names[65]): break;
      case StringHash(names[66]): break;
      case StringHash(names[67]): break;
      case StringHash(names[68]): break;
      case StringHash(names[69]): break;
      case StringHash(names[70]): break;
      case StringHash(names[71]): break;
      case StringHash(names[72]): break;
      case StringHash(names[73]): break;
      case StringHash(names[74]): break;
      case StringHash(names[75]): break;
      case StringHash(names[76]): break;
      case StringHash(names[77]): break;
      case StringHash(names[78]): break;
      case StringHash(names[79]): break;
      case StringHash(names[80]): break;
      case StringHash(names[81]): break;
      case StringHash(names[82]): break;
      case StringHash(names[83]): break;
      case StringHash(names[84]): break;
      case StringHash(names[85]): break;
      case StringHash(names[86]): break;
      case StringHash(names[87]): break;
      case StringHash(names[88]): break;
      case StringHash(names[89]): break;
      case StringHash(names[90]): break;
      case StringHash(names[91]): break;
      case StringHash(names[92]): break;
      case StringHash(names[93]): break;
      case StringHash(names[94]): break;
      case StringHash(names[95]): break;
      case StringHash(names[96]): break;
      case StringHash(names[97]): break;
      case StringHash(names[98]): break;
      case StringHash(names[99]): break;
      default: 
        FAIL();
    }

  }
  
}
