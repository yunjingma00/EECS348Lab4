#include <stdio.h>

int main()
{
    int score;

    while (1)
    {
        printf("Enter the NFL score (Enter 1 to stop): ");
        scanf("%d", &score);

        if (score == 1)
        {
            break;
        }

        printf("Possible combinations of scoring plays if a team's score is %d:\n", score);

        for (int a = 0; a <= score / 8; a++)
        {
            for (int b = 0; b <= score / 7; b++)
            {
                for (int c = 0; c <= score / 6; c++)
                {
                    for (int d = 0; d <= score / 3; d++)
                    {
                        for (int e = 0; e <= score / 2; e++)
                        {
                            if (8 * a + 7 * b + 6 * c + 3 * d + 2 * e == score)
                            {
                                printf("%d TD + 2pt, %d TD + FG, %d TD, %d 3pt FG, %d Safety\n",
                                       a, b, c, d, e);
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;
}
