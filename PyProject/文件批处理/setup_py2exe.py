from distutils.core import setup  
import py2exe  

includes = ["encodings", "encodings.*"]

options = {"py2exe":

            {   "compressed": 1,

                "optimize": 2,

                "includes": includes,

                "bundle_files": 1

            }

          }
setup(options = options,
      zipfile=None,
      windows=[{
          "script":'AppFilesOperator.py',
          'icon_resources':[(1, '1.ico')]
                }]
      ) 
#png2ico 1.ico icon_128x128.png icon_64x64.png icon_48x48.png icon_32x32.png icon_16x16.png
#
#icon_128x128.png icon_64x64.png icon_48x48.png icon_32x32.png icon_16x16.png是下载的图片
#
#1.ico是生成的icon,虽然打开看也还是一个文件, 但是其实是支持多个尺寸的
#
#
#
#