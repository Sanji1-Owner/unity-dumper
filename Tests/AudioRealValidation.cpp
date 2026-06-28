#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <cstring>
#include "../UnityAnalyzer/Unity/Objects/ObjectReader.h"

using namespace UnityAnalyzer::Unity::Objects;

void TestAudioReal() {
    std::cout << "Running AudioRealValidation..." << std::endl;

    SerializedObject obj;
    obj.class_id = 83;
    obj.path_id = 2;
    
    // Fake AudioClip Data: Name(len 4 + "Snd" + pad) + LoadType(1) + IsTracker(0) + Ambisonic(0) + pad + Subsound(0) + Preload(1) + Background(0) + Legacy(0) + pad + Source(0) + Offset(0) + Size(4) + Compression(0)
    std::vector<uint8_t> data = {
        3, 0, 0, 0, 'S', 'n', 'd', 0,
        1, 0, 0, 0, // LoadType
        0, 0, 0, 0, // IsTracker, Ambisonic + pad
        0, 0, 0, 0, // Subsound
        1, 0, 0, 0, // Preload, Background, Legacy + pad
        0, 0, 0, 0, // Source (Empty string len 0)
        0, 0, 0, 0, 0, 0, 0, 0, // Offset
        4, 0, 0, 0, 0, 0, 0, 0, // Size
        0, 0, 0, 0  // Compression
    };
    // Append 4 bytes of dummy PCM
    data.push_back(0x01); data.push_back(0x02); data.push_back(0x03); data.push_back(0x04);
    
    obj.data = data;

    AudioClipReader reader;
    UnityAnalyzer::Core::ResourceEntry entry;
    assert(reader.Parse(obj, entry) == true);
    assert(entry.name == "Snd");
    
    std::filesystem::path test_path = "test_audio";
    assert(reader.Export(entry, obj.data, test_path) == true);
    
    std::ifstream wav_file("test_audio.wav", std::ios::binary);
    assert(wav_file.is_open());
    
    char riff[4], wave[4], fmt[4], data_tag[4];
    wav_file.read(riff, 4);
    assert(std::memcmp(riff, "RIFF", 4) == 0);
    wav_file.seekg(8);
    wav_file.read(wave, 4);
    assert(std::memcmp(wave, "WAVE", 4) == 0);
    wav_file.read(fmt, 4);
    assert(std::memcmp(fmt, "fmt ", 4) == 0);
    
    std::cout << "AudioRealValidation: PASS" << std::endl;
}

int main() {
    TestAudioReal();
    return 0;
}
