/*
  ==============================================================================

	This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "../../HybridApi/Arena/CLip/ClipParser.h"

//==============================================================================
MainComponent::MainComponent()
{
	connect( "127.0.0.1", 7000 ); //todo: throw an error when we can't open the osc port

	check = new TextButton( "Check" );
	check->addListener( this );
	addAndMakeVisible( check );
	check->triggerClick(); //do the first check, this will populate the array of clips we need to trigger
	
	pDay = Time::getCurrentTime().getDayOfYear(); //set the day to the current day

	//start checking 30 times a second. This is probably overkill, but at least we're sure we won't miss anything
	startTimerHz( 30 ); 

	setSize( 600, 400 );
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint( Graphics& g )
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll( getLookAndFeel().findColour( ResizableWindow::backgroundColourId ) );

	g.setFont( Font( 16.0f ) );
	for ( auto trigger : triggers )
	{
		g.setColour( Colours::white );
		g.drawText( trigger->defaultName, 10, 60 + triggers.indexOf( trigger ) * 20, 300, 20, Justification::left, true );
		//todo: sort the triggers based on their time, instead of the order they have in the comp file
		//todo: make this a scrollable list when there are more triggers than the window can hold
		int hh = floor( trigger->time / 60 );
		int mm = trigger->time % 60;

		//if a clip's trigger time has passed, paint it green, otherwise red
		if ( trigger->hasTriggeredToday )
			g.setColour( Colours::green ); 
		else
			g.setColour( Colours::red ); 
		g.drawText( String (hh) + ":" + String(mm), 310, 60 + triggers.indexOf( trigger ) * 20, 300, 20, Justification::left, true );
	}
}

void MainComponent::resized()
{
	check->setBounds( 10, 10, 150, 30 );
}

void MainComponent::timerCallback()
{
	Time curTime = Time::getCurrentTime();

	if ( curTime.getDayOfYear() != pDay ) //if the day has changed
	{
		for ( auto trigger : triggers )
			trigger->hasTriggeredToday = false; //reset all the triggers
		pDay = curTime.getDayOfYear();
		repaint();
	}
	
	for ( auto trigger : triggers )
	{
		if ( curTime.getHours() * 60 + curTime.getMinutes() >= trigger->time && !trigger->hasTriggeredToday ) //fire when necessary
		{
			//make the OSC message
			OSCAddressPattern address = OSCAddressPattern( "/composition/layers/" + String( trigger->layer + 1 ) + "/clips/" + String( trigger->column + 1 ) + "/connect" );
			OSCMessage m( address, 1 );
			trigger->hasTriggeredToday = send( m ); //todo: check if the oscmessage actually arrived and the correct clip is playing
			repaint();
		}
	}
}

void MainComponent::buttonClicked( Button * )
{
	//ClipParser checks the Resolume config file for the current composition and ass file
	//ClipParser is part of HybridAPI: https://github.com/jorisdejong/HybridApi
	//HybridAPI has a bunch of classes and functions to get info from Resolume and to deal with common taskss
	ClipParser parser; 

	triggers.clear();
	for ( auto clip : parser.getClips() ) //getClips parses the current comp file and returns an array with info about the clips
	{
		if ( clip.name != clip.defaultName ) //the name has changed, we're assuming its new name is time
			//todo: make sure the changed name is actually something we can translate to hours * 60 + minutes
		{
			ClipTrigger* trigger = new ClipTrigger();
			//hours * 60 + minutes
			//first chars are hh, last two chars are mm
			trigger->time = clip.name.dropLastCharacters( 2 ).getIntValue() * 60 + clip.name.getLastCharacters( 2 ).getIntValue(); 

			trigger->column = clip.column;
			trigger->layer = clip.layer;
			trigger->defaultName = clip.defaultName; //we'll use it when showing the list of clips to be triggered

			//if the current time is later than the trigger, mark it as already triggered
			Time curTime = Time::getCurrentTime();
			trigger->hasTriggeredToday = ( curTime.getHours() * 60 + curTime.getMinutes() > trigger->time ); 

			triggers.add( trigger );
		}
	}

	repaint();
}
