import urllib.request
import time
import json
from sys import stdout

begin = time.time()


def clarify(r, split_char):
    # print ("Running \"clarify\"...")
    try:
        response = str(r.read())
    except Exception as e:
        print("Something lost...")
        while (checker != 1):
            checker = 0
            try:
                clarify(r, split_char)
            except:
                print("Something still is lost...")
                checker = 0
            else:
                checker = 1

    resp = response.strip().replace("\\n", "").split(split_char)  # Splitting response text into more readable array
    remade = []
    for o in resp:
        remade.append(o.strip())  # Remove any unwanted spaces after string
    return remade


class GetRecipeSites:
    def __init__(self):
        self.page_id = 1
        self.recipes_urls = []
        self.base_url = "https://www.nosalty.hu/receptek?page="
        self.page_count = 476  # page_count = 476

    def get_urls(self, r):
        remade = clarify(r, '<')
        possible_urls = []
        for i in remade:
            if ("/recept/" in i):  # Check if wanted phrase is in url
                possible_urls.append(i)

        for possible in possible_urls:
            if ("class=\"position-relative d-block\"" in possible):
                temp = possible.split(' ')
                self.recipes_urls.append("https://www.nosalty.hu" + temp[3].split('"')[1])

    def run(self):
        while (self.page_id <= self.page_count):
            r = urllib.request.urlopen(f"{self.base_url}{str(self.page_id)}")
            self.get_urls(r)
            print(
                "\b" * 750 + f"Running for {round(time.time() - begin, 1)}s: Page ID: {self.page_id} out of {self.page_count}",
                end="")
            stdout.flush()

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
        self.recipe_id = 0
        self.recipe_data = {
            "id": self.recipe_id,
            "name": "",
            "url": "",
            "ingredients": []
        }
        self.recipes = []
        
        self.ingredients_ids = []
        self.ingredient_id = {
            "name": "",
            "ids": []
        }
        self.ingredients_pre = []
        self.ingredients = []

    def get_ingredients(self, url, urls_length, i):
        self.recipe_data["name"] = ""
        self.recipe_data["url"] = ""
        self.recipe_data["ingredients"] = []

        u = url.split('/')

        self.recipe_id = i + 1
        self.recipe_data["id"] = i + 1
        self.recipe_data["name"] = u[4]
        self.recipe_data["url"] = url

        print("\b" * 1000 + f"Running for {round(time.time() - begin, 1)}s: Urls: {i + 1} out of {urls_length} DONE!",
              end="")
        stdout.flush()

        # Requesting given url
        r = urllib.request.urlopen(f"{url}")
        # Calling clarify function to perform some cleaning in the raw requested data
        remade = clarify(r, '<li')

        # Save ingredients into recipe_data
        for k in remade:
            if ("m-list__item p-2 -dotted -fontSize-16 d-flex justify-content-between pl-5" in k):
                # Splitting with spaces to separate href part of the response
                temp = k.split(' ')

                for i in temp:
                    if ("href" in i):
                        # Splitting separated links to get ingredients
                        temp2 = i.split('/')

                        if (temp2[1] == "alapanyag"):

                            ingredient = temp2[2][:-2]
                            self.ingredients_pre.append(str(ingredient) + ":" + str(self.recipe_id))

                            # Checking if ingredient is already added to list
                            if (ingredient not in self.recipe_data["ingredients"]):
                                # Append ingredients to recipe_data object by cutting the last three characters down
                                self.recipe_data["ingredients"].append(ingredient)

        self.ingredients_pre = list(dict.fromkeys(self.ingredients_pre))

        self.recipe_data["ingredients"].sort() # Sort list by alphabetical order
        return self.recipe_data

    def run(self):
        # Reading saved ingredients list into data
        d = open("temp_db", "r")
        data = d.read()
        d.close()

        # Reading saved url_index
        r = open("url_index", "r")
        i = int(r.read())
        r.close()

        # Dealing with url_index so it won't skip the for loop
        if (i >= len(self.recipes_urls)):

            # Resetting url_index to 0
            w = open("url_index", "w")
            w.write("0")
            w.flush()
            w.close()

            # Clearing temp_db content
            w = open("temp_db", "w")
            w.write("")
            w.flush()
            w.close()

            # Clearing Ingredients IDs
            w = open("ingredients_ids", "w")
            w.write("")
            w.flush()
            w.close()

        # Sort recipes_urls by text length
        recipes_urls = sorted(self.recipes_urls, key=len)
        last_recipe_index = i
        # Make requests to the urls from recipes_urls list
        for k in range(len(recipes_urls) - last_recipe_index):
            # Calling get_ingredients function to save
            full_recipe = str(self.get_ingredients(recipes_urls[i], len(recipes_urls) - last_recipe_index, i)) + "\n"
            data += full_recipe
            i += 1
            # Saving current url_index to url_index file
            u = open("url_index", "w")
            u.write(str(i))
            u.flush()
            u.close()

            # Appending data to temp_db
            t = open("temp_db", "a")
            t.write(full_recipe)
            t.flush()
            t.close()
            time.sleep(0.25)

        self.recipes = data.split('\n')

    def update_db(self):
        i = 0
        # Backup system
        r = open("nosalty.json", "r")
        backup = r.readlines()
        r.close()
        w = open("nosalty.json.backup", "w")
        w.writelines(backup)
        w.flush()
        w.close()

        # Update system
        w = open("nosalty.json", "w")
        self.recipes.remove(self.recipes[len(self.recipes) - 1])
        # print (self.recipes)
        for recipe in self.recipes:
            if (recipe != ''):
                data = json.loads(recipe.replace("'", '"'))
                print (data)
                w.write(str(data) + "\n")
                w.flush()
                # print(data["name"])
        w.close()

        # Update ID System
        while (i < len(self.ingredients_pre)):
            temp1 = self.ingredients_pre[i].split(':')
            save_to = temp1[0] + ":"
            ids = ""
            for k in range(len(self.ingredients_pre)):
                temp2 = self.ingredients_pre[k].split(':')
                if (temp1[0] == temp2[0]):
                    ids += temp2[1] + ";"
            for pre in self.ingredients_pre:
                temp3 = pre.split(':')
                if (temp1[0] == temp3[0]):
                    self.ingredients_pre.remove(pre)
            save_to += ids[:-1]
            self.ingredients.append(save_to)
            i += 1


        self.ingredients.sort()

        w = open("ingredients_ids", "w")
        for i in self.ingredients:
            w.write(i + "\n")
            w.flush()
        w.close()

        # Sorting ingredients_ids to be in proper form for the search engine
        ingredients = []
        r = open("ingredients_ids", "r")
        ingredients = r.readlines()
        r.close()
        ingredients.sort()

        w = open("ingredients_ids", "w")
        for i in ingredients:
            w.write(i)
            w.flush()


print(f"Program started at {begin}\n")
recipes = GetRecipeSites()
recipes.run()
recipes_urls = recipes.get_list_of_urls()

print("\nFinished gathering recipes urls...")
recipe_data = GetRecipeData(recipes_urls)

print("\nStarting to receive recipe data...")
recipe_data.run()

print("\n\nUpdating local Database...\n")
recipe_data.update_db()

end = time.time()

print(f"\nTotal runtime: {end - begin} seconds\n")
