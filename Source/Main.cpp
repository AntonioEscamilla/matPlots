/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"


//==============================================================================
class matPlotsApplication  : public JUCEApplication,
                             public Timer
{
public:
    //==============================================================================
    matPlotsApplication() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    //==============================================================================
    void initialise (const String& commandLine) override{
        //splash screen y espera evento del timer para iniciar
        splash = new SplashScreen ("App Splash",ImageCache::getFromMemory(BinaryData::splash_screen_png,BinaryData::splash_screen_pngSize),true);
        startTimer(2000);
        //mainWindow = new MainWindow (getApplicationName());
    }

    void shutdown() override
    {
        // Add your application's shutdown code here..
        splash = nullptr;
        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const String& commandLine) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }
    
    void timerCallback() override {
        splash->deleteAfterDelay (RelativeTime::seconds(2), false);
        mainWindow = new MainWindow (getApplicationName());
        stopTimer();
    }

    //==============================================================================
    /*
        This class implements the desktop window that contains an instance of
        our MainContentComponent class.
    */
    class MainWindow    : public DocumentWindow
    {
    public:
        MainWindow (String name)  : DocumentWindow (name,Colour(0xff3f3f3f),DocumentWindow::allButtons)
        {
            setContentOwned (new MainContentComponent(), true);
            setUsingNativeTitleBar (true);
            LookAndFeel::setDefaultLookAndFeel (&lookAndFeelV3);
            LookAndFeel& laf = getLookAndFeel();
            laf.setColour (TextButton::buttonColourId, Colours::lightgrey);
            laf.setColour (TextButton::buttonOnColourId, Colours::grey);
            laf.setColour (ToggleButton::textColourId, Colours::white);
            laf.setColour (Slider::rotarySliderFillColourId, Colours::white);
            centreWithSize (getWidth(), getHeight());
            setVisible (true);
            setResizable(true, true);
        }

        void closeButtonPressed() override
        {
            // This is called when the user tries to close this window. Here, we'll just
            // ask the app to quit when this happens, but you can change this to do
            // whatever you need.
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */

    private:
        LookAndFeel_V3 lookAndFeelV3;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    ScopedPointer<MainWindow>       mainWindow;
    SplashScreen*                   splash;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (matPlotsApplication)
