import os
import sys

sys.path.insert(0, './awtk')
import awtk_config as awtk

APP_ROOT = os.path.normpath(os.getcwd())
BIN_DIR = awtk.joinPath(APP_ROOT, 'bin')

os.environ['TK_ROOT'] = awtk.TK_ROOT;
os.environ['BIN_DIR'] = awtk.TK_BIN_DIR;
os.environ['LIB_DIR'] = awtk.TK_LIB_DIR;
os.environ['APP_ROOT'] = APP_ROOT;

INCLUDE_PATH=awtk.CPPPATH + [os.path.join(APP_ROOT, 'src')]

DefaultEnvironment(CCFLAGS = awtk.CCFLAGS, 
  LIBS = awtk.LIBS,
  LIBPATH = awtk.LIBPATH,
  CPPPATH = INCLUDE_PATH,
  LINKFLAGS = awtk.LINKFLAGS,
  TARGET_ARCH=awtk.TARGET_ARCH,
  OS_SUBSYSTEM_CONSOLE=awtk.OS_SUBSYSTEM_CONSOLE,
  OS_SUBSYSTEM_WINDOWS=awtk.OS_SUBSYSTEM_WINDOWS
)

SConscriptFiles=[
  'awtk/tools/common/SConscript', 
  'awtk/tools/theme_gen/SConscript', 
  'awtk/tools/font_gen/SConscript', 
  'awtk/tools/image_gen/SConscript', 
  'awtk/tools/image_resize/SConscript', 
  'awtk/tools/res_gen/SConscript', 
  'awtk/tools/str_gen/SConscript', 
  'awtk/tools/ui_gen/qt_to_xml/SConscript',
  'awtk/tools/ui_gen/xml_to_ui/SConscript',
  'awtk/tools/svg_gen/SConscript',
  ]

SConscript(SConscriptFiles)

os.environ['BIN_DIR'] = BIN_DIR
SConscript('src/SConscript')
SConscript('demos/SConscript')
