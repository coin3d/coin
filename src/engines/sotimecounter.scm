;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
;;; Written by mortene@sim.no, 2000-09-07.

;;; Eval following region

;; Make scene-graph and first viewer
(define text (new-sotext3))
(-> (-> text 'string) 'setValue "x") ; initial value, workaround Bugzilla #193

(define scene-graph text)

(define timecounter (new-sotimecounter))
(-> (-> text 'string) 'connectFrom (-> timecounter 'output))

(define viewer (new-soxtexaminerviewer))
(-> viewer 'setscenegraph scene-graph)
(-> viewer 'show)

;;; End initial eval-region

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; Test input fields of SoTimeCounter engine, play around ;;;;;;;;;;;;;;;;;

(-> (-> timecounter 'min) 'setValue 0)
(-> (-> timecounter 'max) 'setValue 20)
(-> (-> timecounter 'frequency) 'setValue 0.2)
(-> (-> timecounter 'step) 'setValue 1)
(-> (-> timecounter 'syncIn) 'setValue) ; trigger restart at min value

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; Confirmed and potential bugs. ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Demonstrate Bugzilla #192 by first turning the engine off, then
;; back on.
(-> (-> timecounter 'on) 'setValue 0) ; off
(-> (-> timecounter 'on) 'setValue 1) ; on


;; Reproduce Bugzilla #194 by setting the value of
;; SoTimeCounter::reset outside the min-max range.
(begin ; Set up engine for test case.
  (-> (-> timecounter 'min) 'setValue 0)
  (-> (-> timecounter 'max) 'setValue 20)
  (-> (-> timecounter 'frequency) 'setValue 0.2)
  (-> (-> timecounter 'step) 'setValue 1))
;; Play around with this value to reproduce bug.
(-> (-> timecounter 'reset) 'setValue 50)


;; Reproduce Bugzilla #195: negative step values doesn't reverse the
;; counting order.
(-> (-> timecounter 'step) 'setValue -1)


;; FIXME: setting the min and max values in the "wrong" order (so
;; min>max) is not handled properly. Should this work? 20000907 mortene.
(begin
  (-> (-> timecounter 'min) 'setValue 0)
  (-> (-> timecounter 'max) 'setValue 10)
  (-> (-> timecounter 'min) 'setValue 20)
  (-> (-> timecounter 'max) 'setValue 0))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; False alarms (ex-possible bugs) ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; The counter doesn't restart at the modulo value of the step, but at
;; 0. This is confirmed to match the behavior of SGI Open Inventor.
(begin
  (-> (-> timecounter 'min) 'setValue 0)
  (-> (-> timecounter 'max) 'setValue 7)
  (-> (-> timecounter 'frequency) 'setValue 0.4)
  (-> (-> timecounter 'step) 'setValue 2))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; scratch area ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(-> (-> text 'justification) 'setValue SoText3::CENTER)
(-> viewer 'viewAll)
