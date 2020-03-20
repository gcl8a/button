//
//  button.h
//  
//
//  Created by Gregory C Lewin on 2/20/17.
//
//

/*
 * A simple button class with polling. Uses 'proper' debouncing with non-blocking timer
 */

#ifndef BUTTON_H
#define BUTTON_H

class Button
{
private:
    enum BUTTON_STATE {BUTTON_STABLE, BUTTON_UNSTABLE};
    BUTTON_STATE state = BUTTON_STABLE;
    
    uint8_t buttonPin;
    uint8_t activeState = 0; //active LOW by default
    
    uint8_t lastButtonState = 1; //last physical ("true") state
    uint8_t currButtonState = 1; //curr state, may fluctuate due to bouncing
    
    uint32_t lastBounceTime = 0;
    uint32_t debouncePeriod = 10; // in ms
    
public:
    Button(uint8_t pin, uint32_t db = 10) //default to 10 ms debounce
    {
        buttonPin = pin;
        
//        if(usePullup) pinMode(pin, INPUT_PULLUP);
//        else pinMode(pin, INPUT);
        
        debouncePeriod = db;
    }
    
    void Init(bool usePullup = true)
    {
        if(usePullup) pinMode(buttonPin, INPUT_PULLUP);
        else pinMode(buttonPin, INPUT);
    }

    bool CheckButtonPress(void)
    {
        bool retVal = false;
        uint8_t buttonState = digitalRead(buttonPin);
        
        if(currButtonState != buttonState)  //there's been a transistion, so start/continue debouncing
        {
            state = BUTTON_UNSTABLE;
         
            lastBounceTime = millis();      //start/restart the debouncing timer
            currButtonState = buttonState;  //
        }
        
        if(millis() - lastBounceTime >= debouncePeriod)
        {
            if(state == BUTTON_UNSTABLE) state = BUTTON_STABLE;
        }
        
        if(state == BUTTON_STABLE)
        {
            if(lastButtonState != currButtonState)
            {
                if(buttonState == activeState) retVal = true;
                lastButtonState = currButtonState;
            }
        }
        
        return retVal;
    }
    
    //older, cheap debouncing
    
//    bool CheckButtonPressed(void)
//    {
//        bool retVal = 0;
//        uint8_t buttonState = digitalRead(buttonPin);
//
//        if(lastButtonState != buttonState)
//        {
//            delay(debounceTime); //cheap debouncing
//            if(buttonState == 0) retVal = true;
//        }
//
//        lastButtonState = buttonState;
//        return retVal;
//    }
//
//    bool CheckButtonReleased(void)
//    {
//        bool retVal = 0;
//        int buttonState = digitalRead(buttonPin);
//
//        if(lastButtonState != buttonState)
//        {
//            delay(debounceTime); //cheap debouncing
//            if(buttonState == 1) retVal = true;
//        }
//
//        lastButtonState = buttonState;
//        return retVal;
//    }
};

#endif /* button_h */
