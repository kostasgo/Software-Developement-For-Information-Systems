# Software Developement For Information Systems

## Kωνσταντίνος Γκόγκας, 1115201200027
## Νικόλαος Σέντης, 1115200700156

## Εισαγωγή

Στην εργασία αυτή κληθήκαμε να υλοποιήσουμε μια αναδιοργάνωση εγγραφών, έτσι ώστε να μπορούμε να βρίσκουμε εύκολα ποιές ταιριάζουν μεταξύ τους. Πιο συγκεκριμένα, έχουμε έναν φάκελο με αρχεία σε μορφή .json, τα οποία δείχνουν specs φωτογραφικών μηχανών. Επίσης, δίνεται αρχείο .csv το οποίο εκφράζει ποιές εγγραφές ταιριάζουν μεταξύ τους. Το ζητούμενο ήταν να προσπαθήσουμε να ομαδοποιήσουμε τις εγγραφές που ταιριάζουν μεταξύ τους σε μια μορφή κλίκας γράφου, ώστε κάθε κόμβος της κλίκας να έχει πρόσβαση στους υπόλοιπους. Στη συνέχεια υλοποιούμε ένα μοντέλο που αναπαριστά κάθε .json αρχείο σε πίνακα tf-idf, με βάση ένα Bag of Words που δημιουργήσαμε. Αυτούς τους πίνακες τους χρησιμοποιύμε για να κάνουμε train ένα μοντέλο logistic regression, το οποίο αφού εκπαιδευτεί θα πρέπει να βγάζει προβλέψεις για αντιστοιχίες που δέν υπάρχουν.

### How to compile & run:

Το project περιέχει αρχέιο makefile για τη μεταγλώτισσή του, καθώς και ένα bash script για την αξιολόγηση του training που έγινε στο μοντέλο. 

* Μεταγλώτισση: make
* Unit testing: ./test [testName]
* Εκτέλεση: ./modelTraining [-l]
* Αξιολόγηση του training: ./validation.sh
* Καθάρισμα του φακέλου από εκτελέσιμα κ.α: make clean
* Μέτρημα γραμμών/λέξεων κώδικα: make count
* Εμφάνιση συγγραφέων: make info


## Μέρος 1ο: Parsing του Dataset και δημιουργία κλικών

### *** ΣΗΜΑΝΤΙΚΟ!!! *** Αλλαγές που έγιναν

Αρχικά τροποποιήθηκε η δομή του hashtable. Πρωτύτερα δεν είχαμε buckets και αντιμετωπίζαμε τα collisions με σειριακή αναζήτηση, καθώς είχαμε στο μυαλό μας πως τα buckets θα είναι οι κλίκες. Με την προσθήκη των buckets όμως η απόδοση βελτιώθηκε αισθητά.
Επίσης αλλάξαμε τον τρόπο που γινόταν το merge των κλικών, καθώς η μέθοδος που χρησιμοποιούσαμε ήταν λάθος και κατέστρεφε την αναζήτηση. Εν τέλει, καταλήξαμε να ανανεώνουμε τους pointers των κλικών αντί για τα στοιχεία, για αυτό το λόγο όμως τα στοιχεία (specs) αποθηκεύονται ξεχωριστά σε μια συνδεδεμένη λίστα ώστε να μπορούν να διαγραφούν σωστά στο τέλος.

### Υλοποίηση

Ο τρόπος που επιλέξαμε να αντιμετωπίσαμε το πρόβλημα ήταν η χρήση ενός hashtable. Ουσιαστικά, τα buckets του hashtable στην περίπτωσή μας είναι οι κλίκες, όπου κάθε κόμβος κλίκας περιέχει μέσα specs για ένα αρχείο .json. Όταν ξεκινάει το πρόγραμμα, διαβάζει όλους τους φακέλους, κάνει parse τα αρχεία .json στη δομή Specs, κι επειτα τα αποθηκεύει στο hashtable. Το hashtable είναι κατασκευασμένο με τέτοιον τρόπο ώστε κατά την εισαγωγή κάθε spec να μπαίνει σε διαφορετική κλίκα. Κάνουμε hash το κλειδί κι έπειτα, από τη θέση που μας βγάλει, ψάχνουμε μέσα στο bucket σειριακά μέχρι την πρώτη κενή κλίκα. Με αυτό τον τρόπο πετυχαίνουμε να έχουμε σίγουρα ένα στοιχείο σε κάθε κλίκα κατά την εισαγωγή, ενώ η πολυπλοκότητα παραμένει Ο(1). Έπειτα, διαβάζουμε το αρχείο .csv γραμμή-γραμμή, και όπου το label είναι 1, ψάχνουμε τις θέσεις των spec_id στον πίνακα και κάνουμε merge τις κλίκες τους. Τελικά καταλήγουμε σε έναν πίνακα από κλίκες, στις οποίες βρίσκονται τα specs στα οποία υπήρχε 1 στο label του αρχείου .csv.

### Δομές που χρησιμοποιήθηκαν

#### Value

Η δομή Value αποθηκεύει τις τιμές των πεδίων δυναμικά σε μία λίστα, καθώς κάποια πεδία έχουν πάνω από μία τιμές. Υλοποιήσαμε τις παρακάτω συναρτήσεις:

  * void insertValue(Value**, char*): δέχεται σαν όρισμα τη διεύθυνσης μιας λίστα με τιμές ιδιότητας και προσθέτει μια άλλη τιμή, το string που είναι το δεύτερο όρισμα της συνάρτησης, στο τέλος της λίστας.

  * void deleteValue(Value*): δέχεται σαν όρισμα μια λίστα με τιμές ιδιότητας και διαγράφει τα δεδομένα της και την ίδια τη λίστα.

  * void printValue(Value*): δέχεται σαν όρισμα μια λίστα με τιμές ιδιότητας και εκτυπώνει όλες τις τιμές που περιέχει.

#### KV_Pair

  Η δομή αυτή είναι απλά ένα "συμμάζεμα" των τιμών και του κλειδιού ενός πεδίου. Παρακάτω δείχνουμε τις συναρτήσεις του:

  * KV_Pair* createKV(char*, Value*): παίρνει σαν όρισμα ένα string και μια λίστα από τιμές και δημιουργεί το αντίστοιχο ζεύγος όνομα ιδιότητας – λίστα τιμών ιδιότητας. Επιστρέφει έναν δείκτη στο ζεύγος που δημιουργείται.

  * void printKV(KV_Pair*): παίρνει σαν όρισμα έναν δείκτη σε KV_Pair και εκτυπώνει το όνομα της ιδιότητας και όλες τις τιμές της.

  * void deleteKV(KV_Pair*): παίρνει σαν όρισμα έναν δείκτη σε KV_Pair και καταστρέφει όλα τα περιέχομενα του ζεύγους και το ίδιο το ζεύγος.

#### SpecsNode
  
  Λίστα με όλα τα ζεύγη όνομα ιδιότητας-τιμές ιδιότητας για ένα spec.

#### Specs

  Περιέχει το id του spec, τη λίστα με όλα τα ζεύγη ιδιότητας-τιμών του και το μέγεθος της λίστα αυτής.
  
  Για τις δύο παραπάνω δομές, χρηισμοποισαμε τις παρακάτω συναρτήσεις.

  * char* createSpecsID(char*, char*): παίρνει σαν όρισμα δύο strings, το όνομα του φακέλου και το όνομα του αρχείου για ένα spec και επιστρέφει το όνομα του id του spec, όπως απαιτείται.

  * void deleteSpecsId(char*): καταστρέφει το id ενός spec.

  *  Specs* createSpecs(char*): παίρνει σαν όρισμα το string του id ενός spec, δημιουργεί ένα κενό spec και επιστρέφει έναν δείκτη σ’ αυτό.

  *  void insertSpecs(Specs**, KV_Pair*): παίρνει ως όρισμα τη διεύθυνση ενός spec και έναν pointer σε ζεύγος ονόματος-τιμών ιδιότητας και το προσθέτει στο τέλος της λίστας του εν λόγω spec.

  *  void printSpecs(Specs*): παίρνει σαν όρισμα ένα spec και εκτυπώνει το id του και όλα τα ζεύγη ονόματος-τιμών ιδιοτήτων που περιέχει.

  *  void deleteSpecsList(SpecsNode *): παίρνει σαν όρισμα μια λίστα με ζεύγη ονόματος-τιμών ιδιοτήτων και καταστρέφει όλα τα περιεχόμενά της.

  *  void deleteSpecs(Specs*): καταστρέφει ένα spec και όλα τα περιεχόμενά του.
  
  
#### CliqueNode

Αυτή είναι η δομή για τους κόμβους της κλίκας. Περιέχει ένα spec και έναν δείκτη στον επόμενο κόμβο της κλίκας καθώς και έναν δείκτη για τη λίστα των αρνητικών κλικών.

#### Clique

Αυτή είναι δομή είναι μια λίστα με τους κόμβους, που αναπαριστά την κλίκα. Περιέχει επίσης το μέγεθος της κλίκας.

  *  Clique* createClique(): δημιουργεί μια κενή κλίκα και επιστρέφει έναν δείκτη σ’ αυτή.

  *  void insertClique(Clique**, Specs*): παίρνει σαν ορίσματα τη διεύθυνση της κλίκας και ένα spec, το οποίο προσθέτει στο τέλος της κλίκας.

  *  void concatCliqueList(CliqueNode*, CliqueNode*): παίρνει σαν όρισμα δύο λίστες με κόμβους κλίκας και προσαρτά τη δεύτερη στο τέλος της πρώτης

  *  void deleteClique(Clique*): καταστρέφει την κλίκα που δέχεται ως όρισμα, καταστρέφοντας πρώτα τα περιεχόμενά της.

  *  void printClique(Clique*): παίρνει ως όρισμα μια κλίκα και εκτυπώνει τα id όλων των spec που περιέχει.

  *  int isInClique(char*, Clique*): boolean συνάρτηση που επιστρέφει 1 αν το spec id που δέχεται σαν όρισμα περιέχεται στην κλίκα, που δέχεται σαν όρισμα, αλλιώς 0.
  
  *  int compareCliques(Clique*, Clique*): συγκρίνει δύο κλίκες κι επιστρέφει 1 εάν είναι ίδιες, αλλιώς 0.
  
  *  int areNegatives(Clique*, Clique*): εξετάζει εάν οι κλίκες είναι αρνητικές μεταξύ τους.
  
#### BucketData

Η δομή των στοιχείων του bucket. Αποτελείται από μία κλίκα και ένα όνομα.

#### Bucket

Το θεμελιώδες στοιχείο του hashtable. Περιέχει πίνακα από bucketData και αξιοποιεί τις παρακάτω συναρτήσεις:

  
  *  Bucket* createBucket(int): Δημιουργεί ένα κενό Bucket με το δοσμένο μέγεθος.
  *  BucketData* searchBucket(Bucket *, char*): Ψάχνει στο bucket για το string, επιστρέφει ένα κενό κελί αν δεν το βρει.
  *  void printBucket(Bucket*): Τυπώνει το bucket.
  *  void deleteBucket(Bucket*): Διαγράφει το bucket.

#### Hashtable

Το hashtable που χρησιμοποιήσαμε για την αποθήκευση και γρήγορη ανεύρεση των πληροφοριών. Αξιοποιεί τις παρακάτω συναρτήσεις:

  *  Hashtable* createHashtable(int, int): Δημιουργεί ένα κενό hashtable.
  *  unsigned long hash(unsigned char*): Η hash function που χρησιμοποιήσαμε (djb2).
  *  int compress(unsigned long, int): Συμπυκνώνει το αποτέλεσμα της συνάρτησης κατακερματισμόυ στο μέγεθος του πίνακα.
  *  void insertHashtable(Hashtable**, Specs*): Εισάγει ένα στοιχείο στον πίνακα.
  *  void deleteHashtable(Hashtable*): Διαγράφει τον πίνακα.
  *  BucketData* searchHashtable(Hashtable*, char*): Τυπώνει τον πίνακα.
  
#### LinkedList

Μια απλή συνδεδεμένη λίστα που έχει ως στοιχεία Specs. Έχει τετριμμένες συναρτήσεις συνδεδεμένης λίστας που έχουμε χρησιμοποιήσει παντού στην εργασία.


#### Συνάρτηση Specs* parser(char*, char*):

Η συνάρτηση αυτή δέχεται ως όρισμα 2 strings (δηλαδή char*). Με βάση αυτά τα 2 ορίσματα, δημιουργεί το string του filepath, ώστε να ανοίξει το άρχειο, που θέλουμε.

Η μεταβλητή quotation_mark χρησιμοποιείται για την εύρεση quotation marks μέσα στο αρχείο .json. Χρησιμοποιείται εν μέρει σαν μετρητής, αν και κάποιες φορές αυξάνεται απλά, για να πάμε στην επόμενη επανάληψη του loop, προσπερνώντας τις υπόλοιπες δομές ελέγχου, ώστε στον επόμενο έλεγχο να μπούμε σε ένα άλλο loop, που θέλουμε.

Η μετάβλητη brackets χρησιμοποιείται σαν boolean. Η αρχική του κατάσταση είναι 0 (δηλαδή false). Γίνεται 1 (δηλαδή true), όταν βρεθεί στην "αρχή" του τμήματος ενός ζευγαριού ιδιότητας-τιμής, κάτι που σημαίνει ότι η εν λόγω ιδιότητα του spec έχει πολλαπλές αντί μίας τιμής (περίπτωση, την οποία χειριζόμαστε διαφορετικά).

Oι μεταβλητές key_size και value_size χρησιμοποιούνται κάθε φορά για το μέγεθος των strings key (ιδιότητα) και value (τιμή) αντίστοιχα.

Στη μεταβλητή current_character αποθηκεύεται ο τρέχων χαρακτηράς που έχει διαβαστεί από το αρχείο. Το αρχείο το διαβάζουμε χαρακτήρα-χαρακτήρα με τη συνάρτηση fgetc().

Κάθε φορά που ξεκινάει μια επανάληψη για εύρεση ενός ζευγαριού ιδιότητας-τιμής, όλοι οι χαρακτηρές μέχρι να βρεθεί ένα quotation mark είναι αδιάφοροι, άρα μέχρι να βρεθεί δεν γίνεται τίποτα, πέραν του να διαβάζεται ο επόμενος χαρακτήρας.

Όταν βρεθεί πρώτη φορά, σημαίνει ότι από τον επόμενο χαρακτήρα ξεκινάει το string του key (όνομα ιδιότητας), έτσι η μεταβλητή quotation_mark αυξάνεται και γίνεται ένα. Επειδή το quotation mark δεν θέλουμε να περιλαμβάνεται στο string μας, στο τέλος της τρέχουσας επανάληψης αυξάνουμε άλλη μια φορά την εν λόγω μετάβλητη, ώστε να λάβει την τιμή 2 και να αρχίσει από την επόμενη επανάληψη να μπαίνει σε άλλες δομές ελέγχου.

  *  Αν ο αμέσως επόμενως χαρακτήρας είναι κι αυτός quotation mark, το key μας θα είναι ένα κενό string. Οπότε δημιουργούμε το κενό string και αυξάνουμε τη μεταβλητή quotation_mark, ώστε να γίνει 3 και να μην μπει στον βρόχο, της περίπτωσης που το key δεν είναι κενό.

  *  Αν ο αμέσως επόμενος χαρακτήρας είναι οποιοσδήποτε άλλος εκτός από quotation mark, μπαίνουμε στον βρόχο, όπου το quotation_mark ισούται με 2. Σ' αυτό τον βρόχο, "χτίζουμε" το string key. Συγκεκριμένα, διαβάζουμε κάθε επόμενο χαρακτήρα, αυξάνουμε το key_size κατά 1, κάνουμε realloc, ούτως ώστε να δεσμευτεί μνήμη για άλλον έναν char στο string key και προσθέτουμε τον τρέχοντα χαρακτήρα στο τέλος του string. Η διαδικασία αυτή συνεχίζεται, μέχρις ότου βρεθεί άλλο ένα quotation mark, το οποίο σηματοδοτεί και το τέλος του key. Τότε, προσθέτουμε στο string key τον χαρακτήρα τερματισμού του string, και αυξάνουμε τη μεταβλητή quotation_mark, ώστε να γίνει 3 και να βγούμε από τον βρόχο.

Μετά ο επόμενος χρήσιμος που μπορεί να βρεθεί είναι είτε πάλι quotation mark, για την περίπτωση που η τιμή του key είναι μία, είτε left square bracket ([), για την περίπτωση που έχει πολλαπλές τιμές.

  *  Αν βρεθεί πρώτα bracket, κάνουμε τη μεταβλητή bracket 1 (true) και στη συνέχεια μπαίνουμε στον αντίστοιχο βρόχο. Στην αρχή κάθε επανάληψης αυτού του βρόχου αγνοούμε κάθε χαρακτήρα, μέχρι να βρεθεί είτε quotation mark, που αντιστοιχεί στην περίπτωση που έχουμε νέα τιμή για το τρέχον key, είτε right square bracket (]), που αντιστοιχεί στην περίπτωση που δεν έχουμε πια άλλη τιμή.

     Στην περίπτωση νέας τιμής, μπαίνουμε σε μια επανάληψη, όπου διαβάζουμε χαράκτηρες και τους προσθέτουμε στο string temp_value, μέχρι να βρούμε αλλαγή γραμμής. Η αλλαγή γραμμής σημαίνει ότι μόλις τελείωσε η τρέχουσα τιμή. Στη συνέχεια ελέγχουμε τους χαρακτήρες του string temp_value από το τέλος προς την αρχή μέχρι να βρούμε quotation mark και μειώνουμε κάθε φορά το value_size κατά 1. Μετά αντιγράφουμε στο string value το string temp_value μέχρι πριν το τελευταίο quotation mark, ώστε να "ξεφορτωθούμε" τους περιττούς χαρακτήρες στο τέλος του.

  *  Αν αντί για bracket μετά το key βρεθεί quotation mark, σημαίνει ότι αυτό το key δεν έχει πολλαπλές τιμές, αλλά μία μοναδική. Τότε, αυξάνουμε τη μεταβλητή quotation_mark, ώστε να γίνει 4. Στο τέλος της τρέχουσας επανάληψης, την αυξάνουμε άλλη μία φορά, ώστε να γίνει 5. Έτσι, αγνοείται το quotation_mark και ξεκινάμε το χτίσιμο του string από τον επόμενο χαράκτηρα. Στη συνέχεια, "χτίζουμε" το string temp_value και κάνουμε την ίδια διαδικασία, που περιγράφηκε παραπάνω, ώστε να "ξεφορτωθούμε" τους περιττούς χαρακτήρες.

Κάθε φορά που έχουμε μια νέα τιμή έτοιμη, την περνάμε στη μεταβλητή val (τύπου Value*). Όταν έχουν περαστεί όλες οι τιμές του τρέχοντος key στη val, περνάμε στη μεταβλητή pair (τύπου KV_Pair*) το ζεύγος ιδιότητας-τιμής (ή τιμών). Στη συνέχεια, περνάμε αυτό το "ζεύγος" στο τρέχον spec μας με τη συνάρτηση insertSpecs().

Στο τέλος, η συνάρτησή μας επιστρέφει σε Specs* το spec που δημιουργήσαμε.

#### Λοιπές Συναρτήσεις

*  void mergeCliques(Hashtable*, char*, char*): Συγχωνεύει τις κλίκες δύο αρχείων. Πιο συγκεκριμένα, αλλάζει ανάλογα το μέγεθος της πρώτης και κάνει τα μέλη της τη δεύτερης να έχουν σαν κλίκα την πρώτη.

*  void updateNegatives(Hashtable*, char*, char*): Ενημερώνει τις κλίκες δύο αρχείων πως έχουν αρνητική συσχέτιση μεταξύ τους.

*  int countDirectories(char*): επιστρέφει τον αριθμό των υποφακέλων ή των αρχείων που βρίσκονται μέσα στον φάκελο του path του string που δέχεται ως όρισμα.

*  char* createPath(char*, char*): δέχεται ως όρισμα το path ενός φακέλου και ενός υποφακέλου ή αρχείου και επιστρέφει το string του path του υποφακέλου ή του αρχείου.

*  void deletePath(char*): καταστρέφει το string που δέχεται σαν όρισμα.

*  char** createDirTable(int, char*): δέχεται σαν όρισμα των αριθμό των υποφακέλων σε έναν φάκελο και το path του φακέλου και επιστρέφει έναν πίνακα με string, που περιέχει όλα τα path που υπάρχουν κάτω από τον φάκελο.

*  void deleteDirTable(char**, int): Δέχεται σαν όρισμα έναν πίνακα με string και τον αριθμό των string που περιέχει και τα καταστρέφει.

## Μέρος 2ο: Μηχανική μάθηση με Bag Of Words, TF-IDF & Λογιστική Παλινδρόμηση

### Αρνητικές συσχετίσεις

Αρχικά για αυτό το μέρος χρειάστηκε και η υλοποίηση αρνητικών συσχετίσεων μεταξύ των κλικών. Για να γίνει αυτό προσθέσαμε ένα στοιχείο στις κλίκες το οποίο είναι μια λίστα από κλίκες - συγκεκριμένα οι κλίκες οι οποίες είναι αρνητικές με την εκάστοτε. Ορίζονται στο negative-cliques.h και χρησιμοποιούν στάνταρ συναρτήσεις συνδεδεμένων λιστών που υπάρχουν στο υπόλοιπο πρόγραμμα.

### Preprocessing & δημιουργία λεξιλογίου

Πρωτού προχωρήσουμε με το bag of words, έπρεπε να "καθαρίσουμε" κάθε αρχείο specs διάφορες λέξεις. Το υλοποιήσαμε ως εξής: Σε κάθε στοιχείο spec (που συμβολίζει ένα αρχείο json) προσθέσαμε και μια λίστα από λέξεις, που είναι ουσιαστικά οι λέξεις που μας ενδιαφέρουν. Εκεί κρατάμε έναν counter που μετράει πόσες φορές εμφανίζεται η λέξη στο αρχείο, ο οποίος χρησιμοποιείται αργότερα για να δημιουργηθεί το tf score κάθε λέξης. Όλες οι λέξεις μετατρέπονται σε lowercase και φεύγουν χαρακτήρες όπως σημεία στίξης και χαρακτήρες ελέγχου ( \n κλπ). Στη συνέχεια διώχνουμε λέξεις που ανήκουν σε κάποια stopwords, όπως και λέξεις που έχουν μέγεθος 1. Έτσι καταλήγουμε με κάθε spec να εκφράζεται από μια σειρά λέξεων και το πόσες φορές εμφανίζονται μέσα στο αρχείο. Στη συνέχεια δημιουργήσαμε μια δομή λεξιλογίου στα αρχεία words.h & bow.h, την οποία θα αναλύσουμε παρακάτω.

#### Word

Αυτή είναι η δομή μιας λέξης. Περιέχει τη συμβολοσειρά, έναν counter που δείχνει σε πόσα αρχεία εμφανίζεται, το idf της, ένα μέσο tf-idf, καθώς και τη θέση της στον τελικό συρικνωμένο πίνακα.

#### Vocabulary

Αυτή είναι η δομή του λεξιλογίου. Ουσιαστικά πρόκειται για ένα hashtable όπως έχει οριστεί και για τις κλίκες με τη διαφορά, πως αντι για bucketData τα buckets του περιέχουν λέξεις (word). Επίσης μια αλλαγή είναι πως όταν γίνεται insert κάτι που ήδη υπάρχει (πράγμα που δε μας απασχολούσε πριν γιατι κάθε αρχείο είναι μοναδικό), αυξάνεται απλώς ο counter της λέξης. Αξιοποιεί τις παρακάτω συναρτήσεις:


  *  char** createStopWordsTable(): Δημιουργεί τον πίνακα των stopwords.
  *  void deleteStopWordsTable(char**): Διαγράφει τον πίνακα των stopwords.
  *  int isStopword(char*, char**): Ελέγχει εάν μια συμβολοσειρά είναι stopword.
  *  void toLower(char*): Μετατρέπει μια συμβολοσειρά σε lowercase.
  *  void filterSpec(Specs*, char**): Φιλτράρει τις συμβολοσειρές ενός αρχείου από αχρείαστους χαρακτήρες
  *  Vocabulary* createVocabulary(int, int): Δημιουργεί ένα λεξιλόγιο με κενές λέξεις.
  *  void insertVocabulary(Vocabulary ** , char*, double): Εισάγει μια λέξη σε ένα λεξιλόγιο.
  *  int fillVocabulary(Vocabulary ** , ListNode*): Γεμίζει το λεξιλόγιο με τις λέξεις όλων των αρχείων.
  *  void updateScores(Vocabulary*, int): Φτιάχνει τα μέσα tf-idf score κάθε λέξης στο λεξιλόγιο.
  *  Word** shrinkTable(Vocabulary*, double, int*): Δημιουργεί έναν μικρότερο πίνακα από λέξεις, οι οποίες είχαν πάνω από ένα δοσμένο μέσο tf-idf.
  *  void deleteVocabulary(Vocabulary*): Διαγράφει το λεξιλόγιο.
  *  Word* searchVocabulary(Vocabulary*, char*): Ψάχνει για μια λέξει στο λεξιλόγιο.
  *  void printVocabulary(Vocabulary*): Τυπώνει ολόκληρο το λεξιλόγιο.
  
  ### Logistic Regression
  
  Σε αυτό το σημείο και πρίν διαβάσουμε το αρχείο .csv έχουμε καταλήξει με έναν πίνακα από λέξεις. Δημιουργούμε λοιπόν έναν ταξινομητή (classifier) με μέγεθος 2 φορές το μέγεθος του πίνακα + 1 και στη συνέχεια, για κάθε γραμμή του αρχείου, αφού κάνουμε update τις κλίκες, περνάμε στον ταξινομητή τους πίνακες που εκφράζουν το tf-idf κάθε λέξης του αρχείου ως προς κάθε λέξη του πίνακα ενωμένους. Οι νέες αλλαγμένες συναρτήσεις του αρχείου create-output.h, καθώς και η δομή του ταξινομητή εξηγούνται παρακάτω.
  
  #### Classifier
  
  Η δομή που χρησιμοποιήσαμε για να υλοποιήσουμε το logistic regression. Έχει ως μέλη το μέγεθος, έναν πίνακα από τα βάρη καθώς και το ρυθμό εκμάθησης (learning rate) του μοντέλου. Ο πίνακας των w θα έχει μέγεθος 2 φορρές το μέγεθος του συρρικνωμένου λεξιλογίου συν ένα. Τα πρώτα 2n κελιά αντιπροσωπεύουν τις τιμές των 2 αρχείων που θα ενώνονται σε 1, ενώ το τελευταίο αντιπροσωπεύει το b (bias).  Στο αρχείο logistic_regression.h ορίζονται οι εξής συναρτήσεις:
  
  *  Classifier* createClassifier(int, double): Δημιουργεί έναν ταξινομητή δοσμένου μεγέθους και ρυθμού εκμάθησης. Αρχικοποιεί τα βάρη ως 0.
  *  void deleteClassifier(Classifier*): Διαγράφει τον ταξινομητή από τη μνήμη.
  *  double sigmoid(double): Η σιγμοειδής συνάρτηση που χρησιμοποιείται για τη λογιστική παλινδρόμηση.
  *  double hypothesis(double*, double*, int): Η συνάρτηση της λογιστικής παλινδρόμησης.
  *  void gradientDescent(double*, double*, int, int, double): Ανανεώνει τα βάρει χρησιμοποιώντας τον αλγόριθμο ελάττωσης της παραγώγου (gradient descent) για την ελαχιστοποίηση του σφάλματος.
  *  void logisticRegression(Classifier*, double*, int, int): Επαναλαμβάνει τον αλγόριθμο του gradient descent για ένα δοσμένο αριθμό επαναλήψεων.
