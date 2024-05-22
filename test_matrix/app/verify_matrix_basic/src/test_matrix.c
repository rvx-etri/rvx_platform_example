#include "test_matrix.h"
#include "core_dependent.h"
#include "ervp_variable_allocation.h"
#include "ervp_float.h"
#include "ervp_printf.h"

char int08_large_array[LARGE_MATRIX_SIZE][LARGE_MATRIX_SIZE] BIG_DATA_BSS;
short int16_large_array[LARGE_MATRIX_SIZE][LARGE_MATRIX_SIZE] BIG_DATA_BSS;
float float_large_array[LARGE_MATRIX_SIZE][LARGE_MATRIX_SIZE] BIG_DATA_BSS;

void generate_test_matrix(ErvpMatrixInfo* matrix_info, int index)
{
  int i,j;
  int invert = 0;
  int index2;

  switch(matrix_info->datatype)
  {
    case MATRIX_DATATYPE_SINT08:
      {
        char* row_addr = (char*)matrix_info->addr;
        index2 = (index==(-1))? -1 : (index%5);
        for(j=0; j<matrix_info->num_row; j++)
        {
          for(i=0; i<matrix_info->num_col; i++)
          {
            switch(index2)
            {
              case 0:
                row_addr[i] = j*matrix_info->num_col + i + 1 - 4;
                break;
              case 1:
                row_addr[i] = j*matrix_info->num_col + i + 8;
                break;
              case 2:
                row_addr[i] = i*matrix_info->num_col + j - 6;
                break;
              case 3:
                row_addr[i] = ((j*matrix_info->num_col + i + 1)>>3) - 8;
                break;
              case 4:
                row_addr[i] = ((j*matrix_info->num_col + i + 1)&7) - 4;
                break;
              case -1:
                row_addr[i] = 0;
                break;
              default:
                while(1);
            }
            if(invert==2)
            {
              row_addr[i] = -row_addr[i];
              invert = 0;
            }
            else
              invert++;
          }
          row_addr = (char*)(((unsigned int)row_addr) + matrix_info->stride);
        }
      }
      break;
    case MATRIX_DATATYPE_UINT08:
      {
        unsigned char* row_addr = (unsigned char*)matrix_info->addr;
        index2 = (index==(-1))? -1 : (index%5);
        for(j=0; j<matrix_info->num_row; j++)
        {
          for(i=0; i<matrix_info->num_col; i++)
          {
            switch(index2)
            {
              case 0:
                row_addr[i] = j*matrix_info->num_col + i + 1 - 4;
                break;
              case 1:
                row_addr[i] = j*matrix_info->num_col + i + 8;
                break;
              case 2:
                row_addr[i] = i*matrix_info->num_col + j - 6;
                break;
              case 3:
                row_addr[i] = ((j*matrix_info->num_col + i + 1)>>3) - 8;
                break;
              case 4:
                row_addr[i] = ((j*matrix_info->num_col + i + 1)&7) - 4;
                break;
              case -1:
                row_addr[i] = 0;
                break;
              default:
                while(1);
            }
            if(invert==2)
            {
              row_addr[i] = -row_addr[i];
              invert = 0;
            }
            else
              invert++;
          }
          row_addr = (unsigned char*)(((unsigned int)row_addr) + matrix_info->stride);
        }
      }
      break;
    case MATRIX_DATATYPE_SINT16:
      {
        short* row_addr = (short*)matrix_info->addr;
        index2 = (index==(-1))? -1 : (index%5);
        for(j=0; j<matrix_info->num_row; j++)
        {
          for(i=0; i<matrix_info->num_col; i++)
          {
            switch(index2)
            {
              case 0:
                row_addr[i] = j*matrix_info->num_col + i + 1 - 4;
                break;
              case 1:
                row_addr[i] = j*matrix_info->num_col + i + 8;
                break;
              case 2:
                row_addr[i] = i*matrix_info->num_col + j - 6;
                break;
              case 3:
                row_addr[i] = ((j*matrix_info->num_col + i + 1)>>3) - 8;
                break;
              case 4:
                row_addr[i] = ((j*matrix_info->num_col + i + 1)&7) - 4;
                break;
              case -1:
                row_addr[i] = 0;
                break;
              default:
                while(1);
            }
            if(invert==2)
            {
              row_addr[i] = -row_addr[i];
              invert = 0;
            }
            else
              invert++;
          }
          row_addr = (short*)(((unsigned int)row_addr) + matrix_info->stride);
        }
      }
      break;
    case MATRIX_DATATYPE_UINT16:
      {
        unsigned short* row_addr = (unsigned short*)matrix_info->addr;
        index2 = (index==(-1))? -1 : (index%5);
        for(j=0; j<matrix_info->num_row; j++)
        {
          for(i=0; i<matrix_info->num_col; i++)
          {
            switch(index2)
            {
              case 0:
                row_addr[i] = j*matrix_info->num_col + i + 1 - 4;
                break;
              case 1:
                row_addr[i] = j*matrix_info->num_col + i + 8;
                break;
              case 2:
                row_addr[i] = i*matrix_info->num_col + j - 6;
                break;
              case 3:
                row_addr[i] = ((j*matrix_info->num_col + i + 1)>>3) - 8;
                break;
              case 4:
                row_addr[i] = ((j*matrix_info->num_col + i + 1)&7) - 4;
                break;
              case -1:
                row_addr[i] = 0;
                break;
              default:
                while(1);
            }
            if(invert==2)
            {
              row_addr[i] = -row_addr[i];
              invert = 0;
            }
            else
              invert++;
          }
          row_addr = (unsigned short*)(((unsigned int)row_addr) + matrix_info->stride);
        }
      }
      break;
    case MATRIX_DATATYPE_SINT32:
      {
        int* row_addr = (int*)matrix_info->addr;
        index2 = (index==(-1))? -1 : (index%5);
        for(j=0; j<matrix_info->num_row; j++)
        {
          for(i=0; i<matrix_info->num_col; i++)
          {
            switch(index2)
            {
              case 0:
                row_addr[i] = j*matrix_info->num_col + i + 1 - 4;
                break;
              case 1:
                row_addr[i] = j*matrix_info->num_col + i + 8;
                break;
              case 2:
                row_addr[i] = i*matrix_info->num_col + j - 6;
                break;
              case 3:
                row_addr[i] = ((j*matrix_info->num_col + i + 1)>>3) - 8;
                break;
              case 4:
                row_addr[i] = ((j*matrix_info->num_col + i + 1)&7) - 4;
                break;
              case -1:
                row_addr[i] = 0;
                break;
              default:
                while(1);
            }
            if(invert==2)
            {
              row_addr[i] = -row_addr[i];
              invert = 0;
            }
            else
              invert++;
          }
          row_addr = (int*)(((unsigned int)row_addr) + matrix_info->stride);
        }
      }
      break;
    case MATRIX_DATATYPE_UINT32:
      {
        unsigned int* row_addr = (unsigned int*)matrix_info->addr;
        index2 = (index==(-1))? -1 : (index%5);
        for(j=0; j<matrix_info->num_row; j++)
        {
          for(i=0; i<matrix_info->num_col; i++)
          {
            switch(index2)
            {
              case 0:
                row_addr[i] = j*matrix_info->num_col + i + 1 - 4;
                break;
              case 1:
                row_addr[i] = j*matrix_info->num_col + i + 8;
                break;
              case 2:
                row_addr[i] = i*matrix_info->num_col + j - 6;
                break;
              case 3:
                row_addr[i] = ((j*matrix_info->num_col + i + 1)>>3) - 8;
                break;
              case 4:
                row_addr[i] = ((j*matrix_info->num_col + i + 1)&7) - 4;
                break;
              case -1:
                row_addr[i] = 0;
                break;
              default:
                while(1);
            }
            if(invert==2)
            {
              row_addr[i] = -row_addr[i];
              invert = 0;
            }
            else
              invert++;
          }
          row_addr = (unsigned int*)(((unsigned int)row_addr) + matrix_info->stride);
        }
      }
      break;
    case MATRIX_DATATYPE_FLOAT32:
      {
        float* row_addr = (float*)matrix_info->addr;
        index2 = (index==(-1))? -1 : (index%5);
        for(j=0; j<matrix_info->num_row; j++)
        {
          for(i=0; i<matrix_info->num_col; i++)
          {
            switch(index2)
            {
              case 0:
                if((i&3)==0)
                  row_addr[i] = ((float)(j*((int)matrix_info->num_col) + i + 1) - 8.0f) * 4355;
                else
                  row_addr[i] = (float)(j*((int)matrix_info->num_col) + i + 1) - 8.0f;
                break;
              case 1:
                if((i&1)==0)
                  row_addr[i] = ((float)(i*((int)matrix_info->num_col) + j + 1) - 6.0f)/(7.0f*1024.0f*1024.0f*256.0f);
                else
                  row_addr[i] = ((float)(i*((int)matrix_info->num_col) + j + 1) - 6.0f)/3120312;
                break;
              case 2:
                row_addr[i] = ((float)(i*((int)matrix_info->num_col) + j + 1) - 6.0f)/7.0f;
                break;
              case 3:
                row_addr[i] = (float)((int)((j*((int)matrix_info->num_col) + i + 1)>>3) - 8);
                break;
              case 4:
                row_addr[i] = (float)((int)((j*((int)matrix_info->num_col) + i + 1)&7) - 4);
                break;
              case -1:
                row_addr[i] = 0;
                break;
              default:
                while(1);
            }
            if(invert==2)
            {
              row_addr[i] = -row_addr[i];
              invert = 0;
            }
            else
              invert++;
          }
          row_addr = (float*)(((unsigned int)row_addr) + matrix_info->stride);
        }
      }
      break;
    default:
      break;
  }
}

volatile int initizlied_int08 = 0;
volatile int initizlied_int16 = 0;
volatile int initizlied_float = 0;

ErvpMatrixInfo int08_large_info;
ErvpMatrixInfo int16_large_info;
ErvpMatrixInfo float_large_info;

static void __attribute__ ((constructor)) __init_large_matrix_info()
{
	int08_large_info.datatype = MATRIX_DATATYPE_SINT08;
	int08_large_info.num_row = LARGE_MATRIX_SIZE;
	int08_large_info.num_col = LARGE_MATRIX_SIZE;
	int08_large_info.addr = (void*)int08_large_array;
	int08_large_info.stride = sizeof(char)*LARGE_MATRIX_SIZE;

  int16_large_info.datatype = MATRIX_DATATYPE_SINT16;
	int16_large_info.num_row = LARGE_MATRIX_SIZE;
	int16_large_info.num_col = LARGE_MATRIX_SIZE;
	int16_large_info.addr = (void*)int16_large_array;
	int16_large_info.stride = sizeof(short)*LARGE_MATRIX_SIZE;

  float_large_info.datatype = MATRIX_DATATYPE_FLOAT32;
	float_large_info.num_row = LARGE_MATRIX_SIZE;
	float_large_info.num_col = LARGE_MATRIX_SIZE;
	float_large_info.addr = (void*)float_large_array;
	float_large_info.stride = sizeof(float)*LARGE_MATRIX_SIZE;

}

void init_large_array_int08(int index)
{	
  int i, j;
  if(initizlied_int08)
    return;
  generate_testvector(&int08_large_info, index);
  initizlied_int08 = 1;
  flush_cache();
}

void init_large_array_int16(int index)
{	
  int i, j;
  if(initizlied_int16)
    return;
  generate_testvector(&int16_large_info, index);
  initizlied_int16 = 1;
  flush_cache();
}

void init_large_array_float(int index)
{	
  int i, j;
  if(initizlied_float)
    return;
  generate_testvector(&float_large_info, index);
  initizlied_float = 1;
  flush_cache();
}
