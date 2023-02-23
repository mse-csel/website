---
title: "Aspects pratiques : Device Tree"
---

## Device Tree: Génération de son propre DTB hors arborescence

Définition d'une extension du Device Tree

```text
/dts-v1/;
#include "allwinner/sun50i-h5-nanopi-neo-plus2.dts”
/ {
    /delete-node/ leds;
    mydevice {
        compatible = "mydevice";
        attribute = "on";
    };
};
```

Génération du DTB (extension du Makefile)

```Makefile
DTB = mydt.dtb
DTS = $(DTB:.dtb=.dts)
INCL+=-I. -I$(KDIR)/include -I$(KDIR)/arch/arm64/boot/dts

dtb: $(DTB)

$(DTB) : $(DTS)
	ln -s $(KDIR)/arch/arm/boot/dts arm
	-cpp $(INCL) -E -P -x assembler-with-cpp $(DTS) | dtc -I dts -O dtb -o $(DTB) -
	rm arm
```

## Device Tree: Lecture des attributs

```c
int skeleton_drv_probe(struct platform_device* pdev)
{
    struct device_node* dt_node = pdev->dev.of_node;
    if (dt_node) {
        int ret              = 0;
        const char* prop_str = 0;
        ret = of_property_read_string(dt_node, "attribute", &prop_str);
        if (prop_str && ret == 0)
            pr_info("attribute=%s (ret=%d)\n", prop_str, ret);
    }
    // ....
    return 0;
}
```

## Device Tree: Exemple avec des sous-nœuds

``` text
/dts-v1/;
    #include "allwinner/sun50i-h5-nanopi-neo-plus2.dts”
/ {
    /delete-node/ leds;
    
    mydevice {
        compatible = "mydevice";
        #address-cells = <1>;
        #size-cells = <0>;

        attribute = "idle";

        mydevice@0 {
            reg = <0x0>;
            attribute = "on";
        };

        mydevice@1 {
            reg = <0x1>;
            attribute = "off";
        };
    };
};
```

## Device Tree: Exemple avec des sous-nœuds - Lecture

```c
int drv_probe(struct platform_device* pdev)
{
    struct device_node* dt_node = pdev->dev.of_node;
    if (dt_node) {
        const unsigned int* prop_reg = 0;
        struct device_node* child    = 0;
        for_each_available_child_of_node(dt_node, child)
        {
            pr_info("child found: name=%s, fullname=%s\n",
                    child->name,
                    child->full_name);
            prop_reg = of_get_property(child, "reg", NULL);
            if (prop_reg != 0) {
                unsigned long reg = of_read_ulong(prop_reg, 1);
                pr_info("reg:%lu\n", reg);
            }
        }
    }
    // ....
    return 0;
}
```