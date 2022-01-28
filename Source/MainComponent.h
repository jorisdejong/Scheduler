/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent :
	public Component,
	public Timer,
	public OSCSender,
	public Button::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
	class ClipTrigger
	{
	public:
		ClipTrigger() {}
		~ClipTrigger() {}

		int layer = 0;
		int column = 0;
		int time = 0;
		bool hasTriggeredToday = false;
		String defaultName = "Empty";
	};

	OwnedArray<ClipTrigger> triggers;
	std::unique_ptr<TextButton> check;

	int pDay = -1;
 
	// Inherited via Button::Listener
	virtual void buttonClicked( Button * ) override;
	// Inherited via Timer
	virtual void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
	
};
