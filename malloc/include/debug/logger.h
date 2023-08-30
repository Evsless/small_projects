#ifndef __LOGGER_H__
#define __LOGGER_H__
/**********************************************************************************************************************
 *  INCLUDE
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  MACRO
 *********************************************************************************************************************/
#define STDLOG(...) log(__VA_ARGS__)

/**********************************************************************************************************************
 *  ENUM
 *********************************************************************************************************************/
typedef enum component
{
    MALLOC
} component_t;

/**********************************************************************************************************************
 *  STRUCTS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * GLOBAL FUNCTION DECLARATION
 *********************************************************************************************************************/
void log(component_t component, ...);

#endif /* __LOGGER_H__ */