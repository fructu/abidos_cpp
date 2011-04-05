}
"""

def main():
    window = MyDotWindow()
    window.set_filter("fdp")
    window.set_dotcode(dotcode)
    window.connect('destroy', gtk.main_quit)
    gtk.main()

if __name__ == '__main__':
    main()


