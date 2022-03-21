import urllib.request
import time

class GetRecipeSites:
    def __init__(self):
        page_id = 1
        recipes_urls = []
    def get_urls(self, r):
        response = str(r.read())
        resp = response.strip().replace("\\n", "").split('<')
        remade = []
        for o in resp:
            remade.append(o.strip())
        possible_urls = []
        for i in remade:
            if ("/recept/" in i):
                possible_urls.append(i)

        for possible in possible_urls:
            if ("class=\"position-relative d-block\"" in possible):
                temp = possible.split(' ')
                self.recipes_urls.append("https://www.nosalty.hu" + temp[3].split('"')[1])

    def run(self):
        begin = time.time()
        while (self.page_id != 100):
            r = urllib.request.urlopen(f"https://www.nosalty.hu/receptek?page={str(page_id)}")
            self.get_urls(r)
            self.page_id += 1
        end = time.time()

    def display(self):
        for url in self.recipes_urls:
            print(url)
    def check_runtime(self):
        print(f"Time taken to run the program {self.end - self.begin}")

recipes = GetRecipeSites()
recipes.run()
recipes.display()
recipes.check_runtime()
