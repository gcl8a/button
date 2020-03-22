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
    
    uint8_t buttonPin = -1;
    uint8_t activeState = LOW; //active LOW by default
    
    uint8_t currButtonState = HIGH; //last physical ("true") state
    uint8_t tempButtonState = HIGH; //curr state, may fluctuate due to bouncing
    
    uint32_t lastBounceTime = 0;
    uint32_t debouncePeriod = 10; // in ms
    
public:
    Button(uint8_t pin, uint32_t db = 10) //default to 10 ms debounce
    {
        buttonPin = pin;
        debouncePeriod = db;
    }
    
    void Init(bool usePullup = true)
    {
        if(usePullup) pinMode(buttonPin, INPUT_PULLUP);
        else pinMode(buttonPin, INPUT);
        
        currButtonState = tempButtonState = digitalRead(buttonPin);
    }

    bool CheckButtonPress(void)
    {
        bool retVal = false;
        uint8_t buttonState = digitalRead(buttonPin);
        
        if(tempButtonState != buttonState)  //there's been a transistion, so start/continue debouncing
        {
            state = BUTTON_UNSTABLE;
         
            lastBounceTime = millis();      //start/restart the debouncing timer
            tempButtonState = buttonState;  //keep track of the bouncing
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
            if(currButtonState != tempButtonState)
            {
                if(tempButtonState == activeState) retVal = true;
                currButtonState = tempButtonState;
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
