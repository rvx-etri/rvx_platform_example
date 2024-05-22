#include "platform_info.h"
#include "ervp_printf.h"
#include "ervp_core_id.h"
#include "ervp_delay.h"
#include "ervp_image.h"
#include "edge_video_system.h"

#ifdef INCLUDE_JPEG_ENCODER
#include "jpeg_encoder.h"
#endif

#include "ervp_image_file.h"

const int TEST_LCD = 0;

ErvpImage* vframe_out = NULL;
ErvpImage* tiger_image_rgb888 = NULL;
ErvpImage* tiger_image_xbgr8888 = NULL;
ErvpImage* jpeg_input = NULL;
ErvpImage* tiger_image_jpeg = NULL;

int main(void)
{
  if(EXCLUSIVE_ID==0) {
    // setting
    int display_hw;
    int image_resolution;
    int image_format;
    if(TEST_LCD)
    {
      display_hw = DISPLAY_HW_LCD;
      image_resolution = IMAGE_RESOLUTION_1024x600;
    }
    else
    {
      display_hw = DISPLAY_HW_HDMI;
      image_resolution = IMAGE_RESOLUTION_FHD;
    }
    image_format = IMAGE_FMT_XBGR_8888_PACKED;
    //image_format = IMAGE_FMT_RGB_888_PLANE;

    // init
    vframe_out = vframe_alloc(image_format, image_resolution);
    edge_video_system_register_frame(vframe_out, 0, 0);
    edge_video_system_config.display_hw = display_hw;
    edge_video_system_start();

    tiger_image_rgb888 = image_file_read("tiger_small.jpg", tiger_image_rgb888);
    tiger_image_xbgr8888 = image_convert_format(tiger_image_rgb888, tiger_image_xbgr8888, IMAGE_FMT_XBGR_8888_PACKED);
    image_copy(tiger_image_xbgr8888, vframe_out, 0, 0);

#ifdef INCLUDE_JPEG_ENCODER
    // hw convertion
    jpeg_input = image_convert_format(tiger_image_xbgr8888, 0, IMAGE_FMT_RGB_565_PACKED);
    flush_cache();
    jpegenc_convert_to_file(jpeg_input, "tiger_small_hw.jpg");
#endif

    // sw convertion
    image_file_write(tiger_image_rgb888, "tiger_small_sw.jpg", 80);
    //image_save_to_file(tiger_image_rgb888, "tiger_img", IMAGE_FILE_FMT_BMP, 0);
    //image_save_to_file(tiger_image_rgb888, "tiger_img", IMAGE_FILE_FMT_PNG, 0);
    fakefile_prepare_dump(); // required if the core does not reach the end of main

    while(1);
    vom_disable();
  }
  return 0;
}
