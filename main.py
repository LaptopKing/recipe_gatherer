import urllib.request
import time


class GetRecipeSites:
    def __init__(self):
        self.page_id = 1
        self.recipes_urls = []
        self.base_url = "https://www.nosalty.hu/receptek?page="
        self.page_count = 10

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
        self.begin = time.time()
        while (self.page_id != self.page_count):
            r = urllib.request.urlopen(f"{self.base_url}{str(self.page_id)}")
            self.get_urls(r)
            self.page_id += 1
        self.end = time.time()

    def display(self):
        for url in self.recipes_urls:
            print(url)

    def check_runtime(self):
        print(f"Time taken to run the program {self.end - self.begin}")

    def get_back_list_of_urls(self):
        return self.recipes_urls
class GetRecipeIngredients:
    def __init__(self):
        pass

recipes = GetRecipeSites()
recipes.run()
# recipes.display()
recipes.check_runtime()
recipes_urls = recipes.get_back_list_of_urls()
