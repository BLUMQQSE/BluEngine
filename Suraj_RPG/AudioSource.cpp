#include "pch.h"
#include "AudioSource.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Input.h"
#include "Game.h"
namespace bm98
{
AudioSource::AudioSource()
{


}

AudioSource::~AudioSource()
{
	sound.stop();
}

void AudioSource::init()
{
	if(ResourceManager::Instance()->has_sound_buffer(current_audio_clip))
		sound.setBuffer(ResourceManager::Instance()->get_sound_buffer(current_audio_clip));
}

void AudioSource::awake()
{
	//sound.setMinDistance(100);
	//sound.setPitch(2);
	//sound.setLoop(true);


	
	current_audio_clip = "footsteps.wav";
	set_sound_attributes();
	if(play_on_awake)
		sound.play();
	
}

void AudioSource::update()
{
	sound.setPosition(sf::Vector3f(
		game_object->get_center().x,
		game_object->get_center().y,
		0
		));
}

std::vector<Editor::SerializedVar> AudioSource::get_editor_values()
{
	std::vector<Editor::SerializedVar> values;

	values.push_back(Editor::SerializedVar("playing", static_cast<void*>(&playing),
		Var::Type::Bool));
	values.push_back(Editor::SerializedVar("loop", static_cast<void*>(&loop),
		Var::Type::Bool));
	values.push_back(Editor::SerializedVar("muted", static_cast<void*>(&muted),
		Var::Type::Bool));
	values.push_back(Editor::SerializedVar("play_on_awake", static_cast<void*>(&play_on_awake),
		Var::Type::Bool));

	values.push_back(Editor::SerializedVar("current_audio", static_cast<void*>(&current_audio_clip),
		Var::Type::String));


	return values;
}

void AudioSource::play()
{
	if (sound.getStatus() == sf::Sound::Status::Playing)
		return;
	set_sound_attributes();
	sound.play();
}

void AudioSource::pause()
{
	sound.pause();
}

void AudioSource::stop()
{
	sound.stop();
}

void AudioSource::set_sound(std::string sound_file_name)
{
	current_audio_clip = sound_file_name;
}

Json::Value AudioSource::serialize_json()
{
	Json::Value obj;

	obj["current-audio-clip"] = current_audio_clip;

	return obj;
}

void AudioSource::unserialize_json(Json::Value obj)
{
	current_audio_clip = obj["current-audio-clip"].asString();
}

void AudioSource::set_active(bool active)
{
	Component::set_active(active);
	if(!active)
		sound.pause();
	else
	{
		if (sound.getStatus() == sf::Sound::Status::Paused)
			sound.play();
	}

}

void AudioSource::set_sound_attributes()
{
	if (!ResourceManager::Instance()->has_sound_buffer(current_audio_clip))
		return;

	sound.setBuffer(ResourceManager::Instance()->get_sound_buffer(current_audio_clip));
	std::string sound_data = current_audio_clip.substr(0, current_audio_clip.find_last_of(".")).append(".json");

	if (ResourceManager::Instance()->has_sound_data(sound_data))
	{
		Json::Value obj = ResourceManager::Instance()->get_sound_data(sound_data);
		if (!obj["attenuation"].isNull())
			sound.setAttenuation(obj["attenuation"].asFloat() / UNIT_SIZE);
		else
			sound.setAttenuation(DEFAULT_ATTENUATION / UNIT_SIZE);

		if (!obj["loop"].isNull())
			sound.setLoop(obj["loop"].asBool());
		else
			sound.setLoop(DEFAULT_LOOP);

		if (!obj["min-distance"].isNull())
			sound.setMinDistance(obj["min-distance"].asFloat() * UNIT_SIZE);
		else
			sound.setMinDistance(DEFAULT_MIN_DISTANCE * UNIT_SIZE);

		if (!obj["pitch"].isNull())
			sound.setPitch(obj["pitch"].asFloat());
		else
			sound.setPitch(DEFAULT_PITCH);

		if (!obj["play-offset"].isNull())
			sound.setPlayingOffset(sf::Time(sf::seconds(obj["play-offset"].asFloat())));
		else
			sound.setPlayingOffset(sf::Time(sf::seconds(0)));

		if (!obj["relative-listener"].isNull())
			sound.setRelativeToListener(obj["relative-listener"].asBool());
		else
			sound.setRelativeToListener(DEFAULT_RELATIVE_LISTENER);

		if (!obj["volume"].isNull())
			sound.setVolume(
				obj["volume"].asFloat() * 
				Game::get_game_settings().audio_settings.master_volume * 
				Game::get_game_settings().audio_settings.sound_volume
			);
		else
			sound.setVolume(DEFAULT_VOLUME);

		if (!obj["play-on-awake"].isNull())
			play_on_awake = obj["play-on-awake"].asBool();
		else
			play_on_awake = false;
	
		return;
	}

	sound.setMinDistance(DEFAULT_MIN_DISTANCE);
	sound.setPitch(DEFAULT_PITCH);
	sound.setLoop(DEFAULT_LOOP);
	sound.setAttenuation(DEFAULT_ATTENUATION);
	sound.setRelativeToListener(DEFAULT_RELATIVE_LISTENER);
	sound.setPlayingOffset(sf::Time(sf::seconds(0)));
	sound.setVolume(DEFAULT_VOLUME);
	play_on_awake = false;
	//play_on_awake
	
}


}