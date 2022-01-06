#pragma once
#pragma warning(disable : 4996)

// function return values
typedef unsigned int FUNCTION_RETURN_CODE;
#define FUNCTION_RETURN_EVERYTHING_OK   (FUNCTION_RETURN_CODE) 0
#define FUNCTION_RETURN_NOT_FOUND       (FUNCTION_RETURN_CODE) 401

// program return values
typedef int PROGRAM_RETURN_CODE;
#define PROGRAM_RETURN_EVERYTHING_OK    (PROGRAM_RETURN_CODE) 0

// C libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>