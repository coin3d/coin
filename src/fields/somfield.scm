;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
;;; Written by mortene@sim.no, 2000-09-15.

;;; Set up connection SoText3::string<-SoColorIndex::index
;;; (i.e. SoMFString<-SoMFInt32).

(define text (new-sotext3))
(-> text 'ref)
(define colorindex (new-socolorindex))
(-> colorindex 'ref)
(-> (-> text 'string) 'connectFrom (-> colorindex 'index))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; Test SoM*::setNum() ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(set-mfield-values! (-> colorindex 'index) 0 '(0 1 2 3 5 7 11))
(-> (-> colorindex 'index) 'setnum 3)
(-> (-> colorindex 'index) 'set1value 0 42)

;; text->string.getNum() should equal booloperation->a.getNum() at all
;; times.
(-> (-> colorindex 'index) 'getnum)
(-> (-> text 'string) 'getnum)

;; Get fields printed out.
(define writeaction (new-sowriteaction))
(-> writeaction 'apply text)
(-> writeaction 'apply colorindex)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; FIXME: mucho testing of the SoMField should be inserted here. The
;; few bits above were just written ad hoc while actually debugging
;; other parts of the library. 20000915 mortene.



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; scratch area ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(-> (-> text 'justification) 'setValue SoText3::CENTER)
(-> (-> text 'string) 'disconnect)
(-> viewer 'viewAll)
