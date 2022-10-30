#include <button.h>

/**
 * @param pin       The pin connected to the button.
 * @param db        Debounce period, in ms. Defaults to 10 ms.
 * @param active    Whether the button is active HIGH or LOW. Defaults to LOW.
 * */
Button::Button(int8_t pin, uint32_t db, uint8_t active) 
{
    buttonPin = pin;
    debouncePeriod = db;
    activeState = active;
}

/**
 * Initializes the pin.
 * 
 * @param usePullup     Whether or not to use internal pullups. Defaults to true.
 * */
void Button::init(bool usePullup)
{
    if(usePullup) pinMode(buttonPin, INPUT_PULLUP);
    else pinMode(buttonPin, INPUT);
    
    stabButtonPos = tempButtonPos = digitalRead(buttonPin);
}

/**
 * Checks for the event of a button press.
 * */
bool Button::checkButtonPress(void)
{
    if(buttonPin == -1) Serial.println(F("Pin = -1!"));

    bool retVal = false;
    uint8_t currButtonPos = digitalRead(buttonPin);
    
    //either state
    if(tempButtonPos != currButtonPos)  //there's been a transistion, so start/continue debouncing
    {
        state = BUTTON_UNSTABLE;
        
        lastBounceTime = millis();      //start/restart the debouncing timer
        tempButtonPos = currButtonPos;  //keep track of the bouncing
    }

    if(state == BUTTON_UNSTABLE)
    {
        if(millis() - lastBounceTime >= debouncePeriod) //timer has expired
        {
            state = BUTTON_STABLE;
        }
    }

    if(state == BUTTON_STABLE)
    {
        if(stabButtonPos != tempButtonPos) //we have a transision
        {
            if(tempButtonPos == activeState) retVal = true;
            stabButtonPos = tempButtonPos;
        }
    }
    
    return retVal;
}
