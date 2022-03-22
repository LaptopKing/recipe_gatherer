import urllib.request
import time
import json

begin = time.time()

def clarify(r, split_char):
    response = str(r.read())
    resp = response.strip().replace("\\n", "").split(split_char) # Splitting response text into more readable array
    remade = []
    for o in resp:
        remade.append(o.strip()) # Remove any unwanted spaces after string
    return remade

class GetRecipeSites:
    def __init__(self):
        self.page_id = int(get_page_id())
        self.recipes_urls = []
        self.base_url = "https://www.nosalty.hu/receptek?page="
        self.page_count = 476

    def get_urls(self, r):
        remade = clarify(r, '<')
        possible_urls = []
        for i in remade:
            if ("/recept/" in i): # Check if wanted phrase is in url
                possible_urls.append(i)

        for possible in possible_urls:
            if ("class=\"position-relative d-block\"" in possible):
                temp = possible.split(' ')
                self.recipes_urls.append("https://www.nosalty.hu" + temp[3].split('"')[1])

    def run(self):
        while (self.page_id != self.page_count):
            r = urllib.request.urlopen(f"{self.base_url}{str(self.page_id)}")
            self.get_urls(r)
            self.page_id += 1

    def display(self):
        for url in self.recipes_urls:
            print(url)

    def get_list_of_urls(self):
        return self.recipes_urls

class GetRecipeData:
    def __init__(self, recipes_urls):
        self.recipes_urls = recipes_urls
        self.recipe_datas = []
        self.recipe_data = {
            "name":"",
            "url":"",
            "ingredients":[]
        }
        self.recipes = []
    def get_ingredients(self, url):
        self.recipe_data["name"] = ""
        self.recipe_data["url"] = ""
        self.recipe_data["ingredients"] = []
        u = url.split('/')
        self.recipe_data["name"] = u[4]
        self.recipe_data["url"] = url
        try:
            r = urllib.request.urlopen(f"{url}")
        except Exception as e:
            check = 0
            while (check != 1):
                try:
                    r = urllib.request.urlopen(f"{url}")
                except Exception as e:
                    check = 0
                else:
                    check = 1

        remade = clarify(r, '<li')

        for k in remade:
            if ("m-list__item p-2 -dotted -fontSize-16 d-flex justify-content-between pl-5" in k):
                temp = k.split(' ') # Splitting with spaces to separate href part of the response
                for i in temp:
                    if ("href" in i):
                        temp2 = i.split('/') # Splitting separated links to get ingredients
                        if (temp2[1] == "alapanyag"):
                            if (temp2[2][:-2] not in self.recipe_data["ingredients"]): # Checking if ingredient is already added to list
                                self.recipe_data["ingredients"].append(temp2[2][:-2]) # Append ingredients to recipe_data object by cutting the last three characters down
        
        return self.recipe_data

    def run(self):
        i = 1
        data = ""
        for url in self.recipes_urls:
            data += str(self.get_ingredients(url)) + ';'
            # print (f'\n{i}. {self.recipe_data["name"]} : {", ".join(self.recipe_data["ingredients"])}')
            # i += 1
            
        self.recipes = data.split(';')

    def update_db(self):
        # Backup system
        r = open("nosalty.json", "r")
        backup = r.readlines()
        w = open("nosalty.json.backup", "w")
        w.writelines(backup)
        w.flush()
        w.close()

        # Update system
        w = open("nosalty.json", "w")
        self.recipes.remove(self.recipes[len(self.recipes) - 1])
        for recipe in self.recipes:
            data = json.loads(recipe.replace("'", '"'))
            w.write(str(data) + "\n")
            print (data)
        w.flush()
        w.close()
        

recipes = GetRecipeSites()
recipes.run()
recipes_urls = recipes.get_list_of_urls()

recipe_data = GetRecipeData(recipes_urls)
recipe_data.run()
recipe_data.update_db()


end = time.time()

print (f"\nTotal runtime: {end - begin} seconds\n")