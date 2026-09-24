#include <stdio.h>
#include <ctype.h>
int main()
{
    double temperature;

    printf("Enter the temperature: ");
    scanf("%lf", &temperature);

    char originalScale;

    printf("Enter the scale (C, F, or K): ");
    scanf(" %c", &originalScale);
    originalScale=toupper(originalScale);

    double convertedTemperature;

    if (originalScale == 'C') {
        convertedTemperature = temperature;
    }
    else if (originalScale == 'F') {
        convertedTemperature = (temperature - 32) * 5.0 / 9.0;
    }
    else if (originalScale == 'K') {
        convertedTemperature = temperature - 273.15;
    }

    printf("Converted temperature: %.2f C\n", convertedTemperature);

    if (convertedTemperature < 0)
    {
        printf("weather condition:freezing\nadvice:Stay indoors!");
    }
    else if (convertedTemperature >= 0 && convertedTemperature < 10)
    {
        printf("weather condition:cold\nadvice:Wear a jacket!");
    }
    else if (convertedTemperature >= 10 && convertedTemperature < 25)
    {
        printf("weather condition:comfortable\nadvice:Enjoy your day!");
    }
    else if (convertedTemperature >= 25 && convertedTemperature < 35)
    {
        printf("weather condition:hot\nadvice:Drink lots of water!");
    }
    else
    {
        printf("weather condition:extreme heat\nadvice:Stay indoors!");
    }

    return 0;
}
