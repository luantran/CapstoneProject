class SZEWriter():
    def __init__(self):
        pass
    def save(self, filename, oldfile, polydata):
        model_name = "registered_" + oldfile.rsplit("/", 1)[1]
        with open(filename, 'w') as f:
            f.writelines(["BEGIN SZE ASCII",
                         "NbNet=1",
                         "BEGIN IMAGES",
                         "NbImage=1",
                         "BEGIN IMAGE "+model_name,
                          ])

