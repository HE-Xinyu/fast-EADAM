# -*- coding: utf-8 -*-
__author__ = "HE Xinyu"

import pandas as pd
import os

PATH = "./"

if __name__ == "__main__":
    lst = os.listdir(PATH)
    for name in lst:
        if os.path.isfile(name):
            true_name, ext = os.path.splitext(name)
            if ext == ".out":
                df = pd.DataFrame(columns=["seats", "学生福利"])
                for i, content in enumerate(open(name).readlines()):
                    df.loc[i, :] = list(map(float, content.split(" ")))

                df.to_excel(true_name + ".xlsx", index=False)

