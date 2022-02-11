---
title: Home
---
# {{ config.site_name }}

<img src="../assets/images/logo_heiafr_color.png">

Welcome to the website of the lecture "{{ config.site_name }} {{ year }}" for the students
{%- if class_names|length > 1 %}
of classes {{ class_names[0:-1:] | join (', ') }} and {{ class_names | last }}
{%- else %}
the class {{ class_names | first }}
{%- endif %}
of the "Haute école d'ingénierie et d'architecture" of Fribourg.

!!! info "URL for this site"
    <span style="font-size: 1.1rem;">{{ config.site_url }}</span>
