#include <stdlib.h>
#include "types.h"

tip_stolbca func_type(const char *value){
    
    char *end;
    strtoll(value, &end, 10);

    if (*end == '\0' &&  end != value){
        return TYPE_INT;
    }

    strtod(value, &end);

    if(*end == '\0' && end != value){
        return TYPE_DOUBLE;
    }
    return TYPE_INT;
}