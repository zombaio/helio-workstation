/*
    This file is part of Helio Workstation.

    Helio is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Helio is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Helio. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

class AudioMonitor;

#define SPECTROGRAM_BUFFER_SIZE 36
#define SPECTROGRAM_NUM_BANDS 32

class SpectrogramAudioMonitorComponent :
    public Component, private Thread, private AsyncUpdater
{
public:

    explicit SpectrogramAudioMonitorComponent(WeakReference<AudioMonitor> targetAnalyzer);
    ~SpectrogramAudioMonitorComponent() override;

    void setTargetAnalyzer(WeakReference<AudioMonitor> targetAnalyzer);

    //===------------------------------------------------------------------===//
    // Component
    //===------------------------------------------------------------------===//

    void paint(Graphics &g) override;

private:

    void run() override;
    void handleAsyncUpdate() override;
    
    WeakReference<AudioMonitor> audioMonitor;
    
    Atomic<float> peakBuffer[SPECTROGRAM_BUFFER_SIZE];
    Atomic<float> spectrum[SPECTROGRAM_BUFFER_SIZE][SPECTROGRAM_NUM_BANDS];

    Atomic<int> head;
    int skewTime;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrogramAudioMonitorComponent)

};
