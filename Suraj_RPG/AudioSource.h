#pragma once
#include "Component.h"
namespace bm98
{

class AudioSource :
    public Component
{

#define DEFAULT_ATTENUATION 20.f
#define DEFAULT_LOOP false
#define DEFAULT_VOLUME 100.f
#define DEFAULT_PITCH 1.f
#define DEFAULT_RELATIVE_LISTENER false
#define DEFAULT_MIN_DISTANCE 1.f

public:
    AudioSource();
    virtual ~AudioSource();

    virtual void awake() override;
    virtual void update() override;

    virtual std::vector<Editor::SerializedVar> get_editor_values() override;

    void play();
    void pause();
    void stop();

    void set_sound(std::string sound_file_name);

    // Inherited via IData
    virtual Json::Value serialize_json() override;
    virtual void unserialize_json(Json::Value obj) override;

    virtual void set_active(bool active) override;

private:
    sf::Sound sound;

    std::string current_audio_clip;

    bool playing = false;
    bool loop = false;
    bool muted = false;
    bool play_on_awake = false;

    void set_sound_attributes();

};

}
