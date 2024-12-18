#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define MAX 1405

typedef struct
{
    int day;
    int month;
    int year;
    int date;

    double temp;
    double phosphate;
    double silicate;
    double nitrite;
    double nitrate;
    double salinity;
    double oxygen;
}Ocean;

int binaryInterpolationSearch(Ocean* structure, int x, int l, int r);
int min(int a, int b);
int max(int a, int b);
void structureDateConverter(Ocean* structure, int records);
int dateConverter(int d, int m, int y);

int main()
{
    Ocean ocean[MAX];

    //~~~~~~~~~~~~~~~~~~~~~~ This is where we transfer the data from the file to the structure. ~~~~~~~~~~~~~~~~~~~~~~
    FILE *file;

    file = fopen("D:\\Users\\Lefteris\\Distance Learning\\2nd Year\\4th Semester\\Data Structures Project\\PART 1\\Source Files\\SortedOcean.txt", "r");

    if (file == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }

    int read = 0;
    int records = 0;

    //These 3 lines of code are used to read/skip the header file, so we later get only the data.
    const unsigned MAX_LENGTH = 256;
    char header[MAX_LENGTH];

    fgets(header, MAX_LENGTH, file);

    //With these 2 lines of code, we read the whole file and print it line by line.
/*    while (fgets(header, MAX_LENGTH, file))
        printf("%s", header); */

    do
    {
        read = fscanf(file, "%d/%d/%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
                      &ocean[records].day,
                      &ocean[records].month,
                      &ocean[records].year,
                      &ocean[records].temp,
                      &ocean[records].phosphate,
                      &ocean[records].silicate,
                      &ocean[records].nitrite,
                      &ocean[records].nitrate,
                      &ocean[records].salinity,
                      &ocean[records].oxygen);

        if (read == 10)
            records++;  //Rows basically.

        if (read != 10 && !feof(file))
        {
            printf("File format incorrect.\n");
            return 1;
        }

        if (ferror(file))
        {
            printf("Error reading file.\n");
            return 1;
        }

    } while(!feof(file));

    fclose(file);

    structureDateConverter(ocean, records);

    printf("Records read: %d!\n\n", records);
    //~~~~~~~~~~~~~~~~~~~~~~ Up to this point the data from the file has been transfered into the structure. ~~~~~~~~~~~~~~~~~~~~~~

    //~~~~~~~~~~~~~~~~~~~~~~ This is where the Search Algorithm is called. ~~~~~~~~~~~~~~~~~~~~~~

    //Set the initial size and borders of the array of structures that is going to be searched.
    int size = MAX;
    int left = 0;
    int right = size - 1;

    //Get the input date from the user.
    int userYear, userMonth, userDay;
    do {
        printf("Give the date you want searched (d/m/yyyy): ");
        scanf("%d/%d/%d", &userDay, &userMonth, &userYear);
    } while((userYear < 2000||userYear>2019 || userMonth < 1||userMonth>12 || userDay < 1||userDay>31) || (userMonth == 2 && userDay >= 30));

    //Convert it to an algorithm-friendly format.
    int userDate = dateConverter(userDay, userMonth, userYear);

    //Run the Search Algorithm.
    int position = binaryInterpolationSearch(ocean, userDate, left, right);

    if (position != -1)
        printf("\nResult: %d/%d/%d was found in position %d of the array.\n", userDay, userMonth, userYear, position);
    if (position == -1)
        printf("\nResult: %d/%d/%d was not found inside the array.\n", userDay, userMonth, userYear);

    //~~~~~~~~~~~~~~~~~~~~~~ This is where the Search Algorithm ends and we get the position of the date. ~~~~~~~~~~~~~~~~~~~~~~

    if (position != -1)
    {
        printf("\nTemperature (C): %g\n", ocean[position].temp);
        printf("Phosphate: %g\n", ocean[position].phosphate);
    }

}

int binaryInterpolationSearch(Ocean* structure, int x, int l, int r)
{

    int size = r - l + 1;

    int mid, next;

    //In case the number we search for is larger than every number inside the array, then it won't be in the array.
    if (x > structure[MAX-1].date) {
        return -1;
    }

    //In case the number we search for is smaller than every number inside the array, then it won't be in the array.
    if (x < structure[0].date) {
        return -1;
    }

    //No elements in the array
    if (l > r){
        return -1;
    }

    //No elements in the array (l>r), or there is one element and it is not x.
    if (l == r && structure[l].date != x){
        return -1;
    }
    //There is one element in the array and it is x.
    else if (l == r && structure[l].date == x){
        return l;
    }
    //Interpolation Search
    mid = l + ((x - structure[l].date)*(r-l))/(structure[r].date-structure[l].date);

    int i = 1;
    //If our value is bigger than the position we currently are in.
    if (x > structure[mid].date) {
        while (1)
        {
            //Move sqrt(size) to the right for i times.
            next = mid + i * (sqrt(size));
            //If we reach the end to the right or go through x, then break the infinite loop since we have determined the space where x is.
            if (next > r || x < structure[next].date) {
                break;
            }
            //If we fall on next, then we have found the position.
            if (x == structure[next].date) {
                return next;
            }
            i = i + 1;
        }
        /* These are the new left and right ends, which will dictate the new size that we are going to use in the new Binary Interpolation Search, that will work on the new space
           making smaller jumps until x is detected. It works retroactively.*/
        l = mid+(i-1)*sqrt(size)+1;
        r = min(r, next - 1);
        return binaryInterpolationSearch(structure, x, l, r);
    }

    //If our value is smaller than the position we currently are in.
    else if (x < structure[mid].date) {
        while (1)
        {
            //Move sqrt(size) to the left for i times.
            next = mid - i * (sqrt(size));
            //If we reach the end to the left or go through x, then break the infinite loop since we have determined the space where x is.
            if (next < l || x > structure[next].date) {
                break;
            }
            //If we fall on next, then we have found the position.
            if (x == structure[next].date) {
                return next;
            }
            i = i + 1;
        }
        /* These are the new left and right ends, which will dictate the new size that we are going to use in the new Binary Interpolation Search, that will work on the new space
           making smaller jumps until x is detected. It works retroactively.*/
        r = mid-(i-1)*sqrt(size)-1;
        l = max(l, next + 1);
        return binaryInterpolationSearch(structure, x, l, r);
    }
    else
        return mid;
}

//Function to return the smallest number out of two.
int min(int a, int b)
{
    return (a > b) ? b : a;
}

//Function to return the biggest number out of two.
int max(int a, int b)
{
    return (a > b) ? a : b;
}

/* This function takes the day, month and year of all the dates in the file and converts them to a double value (yyyymmdd) so that earlier dates are also smaller in size
   which makes them easy to compare to each other when sorted */
void structureDateConverter(Ocean* structure, int records)
{
    char str[9];
    for (int i = 0; i < records; i++)
    {
        if ((structure[i].day) < 10 && (structure[i].month < 10))
            sprintf(str, "%d%c%d%c%d", structure[i].year, '0', structure[i].month, '0', structure[i].day);
        else if (structure[i].month < 10)
            sprintf(str, "%d%c%d%d", (structure[i].year) , '0', structure[i].month, structure[i].day);
        else if (structure[i].day < 10)
            sprintf(str, "%d%d%c%d", structure[i].year, structure[i].month, '0', structure[i].day);
        else
            sprintf(str, "%d%d%d", structure[i].year, structure[i].month, structure[i].day);

        structure[i].date = atoi(str);
    }

    //Print all the values of structure.date.
/*    for (int i = 0; i < records; i++)
        printf("%d\n", structure[i].date);
*/
}

/* This function takes the day, month and year the user has entered and converts them to a yyyymmdd format that can be searched easily with the algorithm */
int dateConverter(int d, int m, int y)
{
    char str[9];

    if (d < 10 && m < 10)
        sprintf(str, "%d%c%d%c%d", y, '0', m, '0', d);
    else if (m < 10)
        sprintf(str, "%d%c%d%d", y, '0', m, d);
    else if (d < 10)
        sprintf(str, "%d%d%c%d", y, m, '0', d);
    else
        sprintf(str, "%d%d%d", y, m, d);

    return atoi(str);
}
