import random
def main():
    res = random.randint(0,100)
    if (res < 100):
        print("print more than 100")
    else:
        print("less than 100")
    return

if __name__ == "__main__":
    main()