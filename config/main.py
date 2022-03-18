import os

def define_env(env):
    env.variables['solution'] = int(os.getenv("SHOW_SOLUTION") or 0)