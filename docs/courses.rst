Training
========

Course "FAIRroot based software for simulation, reconstruction and analysis of physical experiments"
----------------------------------------------------------------------------------------------------

.. note::
	Данный раздёл находится в разработке!

**Целевая аудитория:** студены и молодые сотрудники - физики, начинающие работать с симуляциями и анализом.

**Задача курса** – дать понимание структуры, функционала, архитектуры и взаимосвязи существующих пакетов Geant4, ROOT, FAIRroot и конкретные реализации последнего; сформировать умение пользоваться базовым функционалом указанных пакетов, а также умение модифицировать имеющееся описание установок с целью проведения конкретного исследования.

**Предварительные требования:** знакомство со основными (список прилагается) понятиями C++ и умение читать код 

Литература
~~~~~~~~~~
#. C++

	#. :download:`Справочник по С++ <_bin/shildt.pdf>`
	#. :download:`Учебник по С++ <_bin/ekkel.pdf>`
	#. `Базовые знания по компиляции <http://knzsoft.ru/cpp-bgr-ls1/>`_ 
	#. `Минимальные знания, которые предъявляет ROOT <http://root.cern.ch/root/htmldoc/guides/users-guide/ROOTUsersGuideChapters/ALittleC++.pdf>`_   

#. ROOT

	#. `Документация ROOT <https://root.cern.ch/guides/users-guide>`_ 
	#. `A ROOT guide for beginners. <https://root.cern.ch/root/htmldoc/guides/primer/ROOTPrimerLetter.pdf>`_  
	#. `Методичка: "Информационные методы в физике высоких энергий" <http://lib.sinp.msu.ru/static/tutorials/141_Leontiev_Zadahi_2011.pdf>`_

#. Geant4.

	#. `Документация Geant4 <https://geant4.web.cern.ch/geant4/support/userdocuments.shtml>`_

#. FairRoot.

	#. `How to <https://fairroot.gsi.de/?q=node/27>`_ 
	#. `Примеры <https://github.com/FairRootGroup/FairRoot/tree/master/examples>`_   

Минимальные требования знаний по С++:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#. Компилятор GNU g++. Компиляция программы из одного или нескольких исходных файлов. 
#. Инструмент cmake. Ознакомиться с материалом: :ref:`cmake_struct`.
#. Выражения: Основные типы. Инициализация переменных. Арифметические и логические операторы. Инкрементация и декрементация. Указатели. Операторы взятия адреса и разыменования.
#. Функции: Объявление. Определение. Вызов. Области видимости. Аргументы функции main.
#. Операторы: Условный оператор. Циклы. Операторы перехода: return, break, continue.
#. Массивы: Одномерные массивы, указатель на массив. Передача в функцию.
#. Ввод-вывод из stl. Консольный: cout, cin, cerr. Файловые потоки ввода-вывода.
#. Классы и объекты. Конструктор, деструктор, конструтктор копирования, методы, атрибуты. private и public. Статические члены класса. Указатель this. Передача объекта в функцию и возвращение из функции.
#. Наследование. Общее понимание.
#. Классы vector и map, list и их итераторы из stl.

Методичка по "лайфхакам"
~~~~~~~~~~~~~~~~~~~~~~~~

#. G++. Компиляция исполняемого файла, статической библиотеки и динамической библиотеки. Добавление директорий для #include. Линковка с библиотеками.
#. Минимальный makefile
