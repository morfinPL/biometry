//=======================================================================
/** @file AudioFile.h
 *  @author Adam Stark
 *  @copyright Copyright (C) 2017  Adam Stark
 *
 * This file is part of the 'AudioFile' library
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
//=======================================================================

#ifndef LIBS_AUDIOFILE_INCLUDE_AUDIOFILE_H_
#define LIBS_AUDIOFILE_INCLUDE_AUDIOFILE_H_

#include <algorithm>
#include <assert.h>
#include <iostream>
#include <string>
#include <vector>

//=============================================================
/** The different types of audio file, plus some other types to
 * indicate a failure to load a file, or that one hasn't been
 * loaded yet
 */
enum class AudioFileFormat
{
    Error,
    NotLoaded,
    Wave,
    Aiff
};

//=============================================================
template <class T>
class AudioFile
{
   public:
    //=============================================================
    typedef std::vector<std::vector<T>> AudioBuffer;

    //=============================================================
    /** Constructor */
    AUDIOFILE_DLL_API AudioFile();

    //=============================================================
    /** Loads an audio file from a given file path.
     * @Returns true if the file was successfully loaded
     */
    AUDIOFILE_DLL_API bool load(const std::string& filePath);

    /** Saves an audio file to a given file path.
     * @Returns true if the file was successfully saved
     */
    AUDIOFILE_DLL_API bool save(const std::string& filePath, AudioFileFormat format = AudioFileFormat::Wave);

    //=============================================================
    /** @Returns the sample rate */
    AUDIOFILE_DLL_API int64_t getSampleRate() const;

    /** @Returns the number of audio channels in the buffer */
    AUDIOFILE_DLL_API int getNumChannels() const;

    /** @Returns true if the audio file is mono */
    AUDIOFILE_DLL_API bool isMono() const;

    /** @Returns true if the audio file is stereo */
    AUDIOFILE_DLL_API bool isStereo() const;

    /** @Returns the bit depth of each sample */
    AUDIOFILE_DLL_API int getBitDepth() const;

    /** @Returns the number of samples per channel */
    AUDIOFILE_DLL_API int getNumSamplesPerChannel() const;

    /** @Returns the length in seconds of the audio file based on the number of samples and sample rate */
    AUDIOFILE_DLL_API double getLengthInSeconds() const;

    /** Prints a summary of the audio file to the console */
    AUDIOFILE_DLL_API void printSummary() const;

    //=============================================================

    /** Set the audio buffer for this AudioFile by copying samples from another buffer.
     * @Returns true if the buffer was copied successfully.
     */
    AUDIOFILE_DLL_API bool setAudioBuffer(AudioBuffer& newBuffer);

    /** Sets the audio buffer to a given number of channels and number of samples per channel. This will try to preserve
     * the existing audio, adding zeros to any new channels or new samples in a given channel.
     */
    AUDIOFILE_DLL_API void setAudioBufferSize(int numChannels, int numSamples);

    /** Sets the number of samples per channel in the audio buffer. This will try to preserve
     * the existing audio, adding zeros to new samples in a given channel if the number of samples is increased.
     */
    AUDIOFILE_DLL_API void setNumSamplesPerChannel(int numSamples);

    /** Sets the number of channels. New channels will have the correct number of samples and be initialised to zero */
    AUDIOFILE_DLL_API void setNumChannels(int numChannels);

    /** Sets the bit depth for the audio file. If you use the save() function, this bit depth rate will be used */
    AUDIOFILE_DLL_API void setBitDepth(int numBitsPerSample);

    /** Sets the sample rate for the audio file. If you use the save() function, this sample rate will be used */
    AUDIOFILE_DLL_API void setSampleRate(uint32_t newSampleRate);

    //=============================================================
    /** A vector of vectors holding the audio samples for the AudioFile. You can
     * access the samples by channel and then by sample index, i.e:
     *
     *      samples[channel][sampleIndex]
     */
    AudioBuffer samples;

   private:
    //=============================================================
    enum class Endianness
    {
        LittleEndian,
        BigEndian
    };

    //=============================================================
    AudioFileFormat determineAudioFileFormat(std::vector<uint8_t>& fileData);
    bool decodeWaveFile(std::vector<uint8_t>& fileData);
    bool decodeAiffFile(std::vector<uint8_t>& fileData);

    //=============================================================
    bool saveToWaveFile(std::string filePath);
    bool saveToAiffFile(std::string filePath);

    //=============================================================
    void clearAudioBuffer();

    //=============================================================
    int32_t fourBytesToInt(std::vector<uint8_t>& source, int startIndex, Endianness endianness = Endianness::LittleEndian);
    int16_t twoBytesToInt(std::vector<uint8_t>& source, int startIndex, Endianness endianness = Endianness::LittleEndian);
    int getIndexOfString(std::vector<uint8_t>& source, std::string s);

    //=============================================================
    T sixteenBitIntToSample(int16_t sample);
    int16_t sampleToSixteenBitInt(T sample);

    //=============================================================
    uint8_t sampleToSingleByte(T sample);
    T singleByteToSample(uint8_t sample);

    int64_t getAiffSampleRate(std::vector<uint8_t>& fileData, int sampleRateStartIndex);
    bool tenByteMatch(std::vector<uint8_t>& v1, int startIndex1, std::vector<uint8_t>& v2, int startIndex2);
    void addSampleRateToAiffData(std::vector<uint8_t>& fileData, int64_t sampleRate);
    T clamp(T v1, T minValue, T maxValue);

    //=============================================================
    void addStringToFileData(std::vector<uint8_t>& fileData, std::string s);
    void addInt32ToFileData(std::vector<uint8_t>& fileData, int32_t i, Endianness endianness = Endianness::LittleEndian);
    void addInt16ToFileData(std::vector<uint8_t>& fileData, int16_t i, Endianness endianness = Endianness::LittleEndian);

    //=============================================================
    bool writeDataToFile(std::vector<uint8_t>& fileData, std::string filePath);

    //=============================================================
    AudioFileFormat audioFileFormat;
    int64_t sampleRate;
    int bitDepth;
};

#endif  // LIBS_AUDIOFILE_INCLUDE_AUDIOFILE_H_
