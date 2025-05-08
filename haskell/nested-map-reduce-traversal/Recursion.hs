type LessonPosition = Int; type SectionPosition = Int

data Section = Section {
  title :: String, resetLessonPosition :: Bool,
  position :: SectionPosition, lessons :: [Lesson]
} deriving Show

data Lesson = Lesson {
  name :: String,
  lessonPosition :: LessonPosition
} deriving Show

updateSections :: [Section] -> SectionPosition -> LessonPosition -> [Section]
updateSections [] _ _ = []
updateSections (s:ss) sp lp =
  s {position = sp', lessons = ls} : updateSections ss sp' (lessonPosition (last ls))
  where sp' = sp + 1
        ls = updateLessons (lessons s) (if resetLessonPosition s == True then 0 else lp)

updateLessons :: [Lesson] -> LessonPosition -> [Lesson]
updateLessons [] _ = []
updateLessons (l:ls) lp = l {lessonPosition = lp'} : updateLessons ls lp'
  where lp' = lp + 1

sections = [Section "Getting started" False 0 [Lesson "Welcome" 0, Lesson "Installation" 0],
            Section "Basic operator" False 0 [Lesson "Addition / Subtraction" 0, Lesson "Multiplication / Division" 0],
            Section "Advanced topics" True 0 [Lesson "Mutability" 0, Lesson "Immutability" 0]]

main = print $ updateSections sections 0 0
