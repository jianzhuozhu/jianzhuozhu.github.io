cmake_minimum_required(VERSION 3.5.1 FATAL_ERROR)
project(img_show)
add_compile_options(-std=c++14)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})



add_executable(Smooth_spatial_filtering src/Smooth_spatial_filtering.cpp)      
target_link_libraries(Smooth_spatial_filtering ${OpenCV_LIBS})



add_executable(fastFuriorTransform src/fastFuriorTransform.cpp)      
target_link_libraries(fastFuriorTransform ${OpenCV_LIBS})


add_executable(ideal_lbrf_kernel src/ideal_lbrf_kernel.cpp)      
target_link_libraries(ideal_lbrf_kernel ${OpenCV_LIBS})


add_executable(img_cor src/img_cor.cpp)      
target_link_libraries(img_cor ${OpenCV_LIBS})

add_executable(img_can src/img_can.cpp)      
target_link_libraries(img_can ${OpenCV_LIBS})


add_executable(canny src/canny.cpp)      
target_link_libraries(canny ${OpenCV_LIBS})

add_executable(hough_l src/hough_l.cpp)      
target_link_libraries(hough_l ${OpenCV_LIBS})

add_executable(hough_c src/hough_c.cpp)      
target_link_libraries(hough_c ${OpenCV_LIBS})





add_executable(hough_c_src src/hough_c_src.cpp)      
target_link_libraries(hough_c_src ${OpenCV_LIBS})

add_executable(RGB_HSV src/RGB_HSV.cpp)      
target_link_libraries(RGB_HSV ${OpenCV_LIBS})

add_executable(detect_leg src/detect_leg.cpp)      
target_link_libraries(detect_leg ${OpenCV_LIBS})

add_executable(circlefind src/circlefind.cpp)      
target_link_libraries(circlefind ${OpenCV_LIBS})

add_executable(ncc src/ncc.cpp)      
target_link_libraries(ncc ${OpenCV_LIBS})
