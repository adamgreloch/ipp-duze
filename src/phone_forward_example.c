#ifdef NDEBUG
#undef NDEBUG
#endif

#include "phone_forward.h"
#include <assert.h>
#include <string.h>

#define MAX_LEN 30

int main() {
    char num1[MAX_LEN + 1], num2[MAX_LEN + 1];
    PhoneForward *pf;
    PhoneNumbers *pnum;

    pf = phfwdNew();

    strcpy(num1, "123");
    strcpy(num2, "9");
    assert(phfwdAdd(pf, num1, num2) == true);
    memset(num1, 0, sizeof num1);
    memset(num2, 0, sizeof num2);

    pnum = phfwdGet(pf, "1234");
    assert(strcmp(phnumGet(pnum, 0), "94") == 0);
    assert(phnumGet(pnum, 1) == NULL);
    phnumDelete(pnum);

    pnum = phfwdGet(pf, "12");
    assert(strcmp(phnumGet(pnum, 0), "12") == 0);
    phnumDelete(pnum);

    strcpy(num1, "123456");
    strcpy(num2, "777777");
    assert(phfwdAdd(pf, num1, num2) == true);

    pnum = phfwdGet(pf, "12345");
    assert(strcmp(phnumGet(pnum, 0), "945") == 0);
    phnumDelete(pnum);

    pnum = phfwdGet(pf, "123456");
    assert(strcmp(phnumGet(pnum, 0), "777777") == 0);
    phnumDelete(pnum);

    pnum = phfwdGet(pf, "997");
    assert(strcmp(phnumGet(pnum, 0), "997") == 0);
    phnumDelete(pnum);

    assert(phfwdAdd(pf, "431", "432") == true);
    assert(phfwdAdd(pf, "432", "433") == true);
    pnum = phfwdGet(pf, "431");
    assert(strcmp(phnumGet(pnum, 0), "432") == 0);
    phnumDelete(pnum);
    pnum = phfwdGet(pf, "432");
    assert(strcmp(phnumGet(pnum, 0), "433") == 0);

    phnumDelete(pnum);
    phfwdDelete(pf);

    pf = phfwdNew();
    pnum = phfwdGet(pf, "02");
    assert(strcmp(phnumGet(pnum, 0), "02") == 0);
    phnumDelete(pnum);
    pnum = phfwdGet(pf, "022");
    assert(strcmp(phnumGet(pnum, 0), "022") == 0);
    phnumDelete(pnum);
    phfwdRemove(pf, "01");
    phfwdRemove(pf, "0221");
    phfwdRemove(pf, "021");
    pnum = phfwdGet(pf, "002");
    assert(strcmp(phnumGet(pnum, 0), "002") == 0);
    phnumDelete(pnum);
    phfwdRemove(pf, "22122");
    pnum = phfwdGet(pf, "02");
    assert(strcmp(phnumGet(pnum, 0), "02") == 0);
    phnumDelete(pnum);
    phfwdRemove(pf, "1");
    pnum = phfwdGet(pf, "002");
    assert(strcmp(phnumGet(pnum, 0), "002") == 0);
    phnumDelete(pnum);
    phfwdRemove(pf, "01020");
    phfwdRemove(pf, "02110");
    strcpy(num1, "2");
    strcpy(num2, "020");
    assert(phfwdAdd(pf, num1, num2) == true);
    phfwdRemove(pf, "21");
    phfwdRemove(pf, "20122");
    pnum = phfwdGet(pf, "102");
    assert(strcmp(phnumGet(pnum, 0), "102") == 0);
    phnumDelete(pnum);
    phfwdRemove(pf, "20220");
    phfwdRemove(pf, "0");
    phfwdRemove(pf, "1021");
    phfwdRemove(pf, "21");
    pnum = phfwdGet(pf, "12011");
    assert(strcmp(phnumGet(pnum, 0), "12011") == 0);
    phnumDelete(pnum);
    phfwdRemove(pf, "121");
    pnum = phfwdGet(pf, "11200");
    assert(strcmp(phnumGet(pnum, 0), "11200") == 0);
    phnumDelete(pnum);
    pnum = phfwdGet(pf, "1211");
    assert(strcmp(phnumGet(pnum, 0), "1211") == 0);
    phnumDelete(pnum);
    strcpy(num1, "00022");
    strcpy(num2, "1");
    assert(phfwdAdd(pf, num1, num2) == true);
    phfwdRemove(pf, "20010");
    phfwdRemove(pf, "22");
    pnum = phfwdGet(pf, "1201");
    assert(strcmp(phnumGet(pnum, 0), "1201") == 0);
    phnumDelete(pnum);
    pnum = phfwdGet(pf, "0");
    assert(strcmp(phnumGet(pnum, 0), "0") == 0);
    phnumDelete(pnum);
    pnum = phfwdGet(pf, "02");
    assert(strcmp(phnumGet(pnum, 0), "02") == 0);
    phnumDelete(pnum);
    strcpy(num1, "2002");
    strcpy(num2, "12212");
    assert(phfwdAdd(pf, num1, num2) == true);
    strcpy(num1, "21001");
    strcpy(num2, "21");
    assert(phfwdAdd(pf, num1, num2) == true);
    strcpy(num1, "001");
    strcpy(num2, "210");
    assert(phfwdAdd(pf, num1, num2) == true);
    phfwdRemove(pf, "200");
    pnum = phfwdGet(pf, "12120");
    assert(strcmp(phnumGet(pnum, 0), "12120") == 0);
    phnumDelete(pnum);
    phfwdRemove(pf, "11212");
    strcpy(num1, "201");
    strcpy(num2, "220");
    assert(phfwdAdd(pf, num1, num2) == true);
    phfwdRemove(pf, "00");
    phfwdRemove(pf, "20012");
    pnum = phfwdGet(pf, "2");
    phnumDelete(pnum);
    phfwdRemove(pf, "1011");
    phfwdRemove(pf, "00");
    pnum = phfwdGet(pf, "0010");
    assert(strcmp(phnumGet(pnum, 0), "0010") == 0);
    phnumDelete(pnum);
    pnum = phfwdGet(pf, "0122");
    assert(strcmp(phnumGet(pnum, 0), "0122") == 0);
    phnumDelete(pnum);
    strcpy(num1, "210");
    strcpy(num2, "1012");
    assert(phfwdAdd(pf, num1, num2) == true);
    strcpy(num1, "00");
    strcpy(num2, "22121");
    assert(phfwdAdd(pf, num1, num2) == true);
    phfwdRemove(pf, "022");
    pnum = phfwdGet(pf, "21");
    assert(strcmp(phnumGet(pnum, 0), "0201") == 0);
    phnumDelete(pnum);
    strcpy(num1, "012");
    strcpy(num2, "1120");
    assert(phfwdAdd(pf, num1, num2) == true);
    pnum = phfwdGet(pf, "201");
    phnumDelete(pnum);
    phfwdRemove(pf, "02");
    strcpy(num1, "0");
    strcpy(num2, "00");
    assert(phfwdAdd(pf, num1, num2) == true);
    phfwdRemove(pf, "201");
    phfwdRemove(pf, "0210");
    pnum = phfwdGet(pf, "002");
    assert(strcmp(phnumGet(pnum, 0), "221212") == 0);

    phnumDelete(pnum);
    phfwdDelete(pf);
}
