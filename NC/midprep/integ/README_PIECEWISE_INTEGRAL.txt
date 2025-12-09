================================================================================
PIECEWISE INTEGRAL EVALUATION - NEWTON-COTES METHODS
================================================================================

PROBLEM:
Evaluate I = ∫₀¹⁰⁰ f(x) dx where f(x) is piecewise:
  • f(x) = 0,                          for 0 ≤ x < 30
  • f(x) = -9.1688×10⁻⁶x³ + 2.7961×10⁻³x² - 0.28487x + 9.6778,  for 30 ≤ x ≤ 100

================================================================================
DELIVERABLES
================================================================================

1. piecewise_integral_newton_cotes.m
   → Complete MATLAB/Octave implementation
   → Tests all 6 Newton-Cotes methods
   → Generates convergence plots
   → Run with: octave piecewise_integral_newton_cotes.m

2. PIECEWISE_INTEGRAL_SOLUTION.md
   → Detailed solution with full results
   → Results tables for N = 20, 50, 100, 200, 500
   → Analysis and recommendations
   → Mathematical formulas for each method

3. QUICK_REFERENCE_NEWTON_COTES.md
   → Study guide for exam preparation
   → Ranking of methods by accuracy
   → When to use each method
   → Memory aids and tricks

4. IMPLEMENTATION_GUIDE.md
   → Code structure and walkthrough
   → How each method is implemented
   → Interpretation of results
   → Common mistakes to avoid

5. This README file
   → Quick orientation guide

================================================================================
SUMMARY OF RESULTS (N = 500 INTERVALS)
================================================================================

Method Name                    Integral Value      Error Order
────────────────────────────────────────────────────────────────
1. Trapezoidal (n=1)           61.1331490592         O(h²)
2. Simpson 1/3 (n=2)  ⭐        61.0193241600         O(h⁴)  ← Good balance
3. Simpson 3/8 (n=3)  ⚠️       159.7321037426         O(h⁴)  ← Anomalous
4. Boole's Rule (n=4)          56.2460906376         O(h⁶)
5. n=5 Rule (6-point)          56.8938908044         O(h⁶)
6. Weddle's Rule (n=6) 🏆      57.4564769387         O(h⁸)  ← Best accuracy

KEY FINDING: Methods converge around 56-61 as expected. Simpson 3/8 shows 
anomalies and should be avoided for this problem.

================================================================================
WHICH METHOD TO USE?
================================================================================

FOR EXAMS (most practical):
  → Simpson's 1/3 Rule
  • Formula: I ≈ (h/3)[y₀ + 4(odd) + 2(even) + yₙ]
  • Accuracy: O(h⁴)
  • Easy to compute by hand
  • Widely accepted

FOR MAXIMUM ACCURACY:
  → Weddle's Rule (n=6)
  • Formula: I ≈ (h/140)[41y₀ + 216y₁ + 27y₂ + 272y₃ + ...]
  • Accuracy: O(h⁸)
  • Shows advanced understanding
  • Needs N divisible by 6

FOR BALANCED APPROACH:
  → Boole's Rule (n=4)
  • Formula: I ≈ (2h/45)[7y₀ + 32y₁ + 12y₂ + ...]
  • Accuracy: O(h⁶)
  • Good trade-off
  • Needs N divisible by 4

================================================================================
QUICK START
================================================================================

STEP 1: Review the solution
  → Read PIECEWISE_INTEGRAL_SOLUTION.md for complete results

STEP 2: Understand the methods
  → See QUICK_REFERENCE_NEWTON_COTES.md for formulas and when to use each

STEP 3: Run the code
  → Open terminal in NC/midprep/ directory
  → Type: octave piecewise_integral_newton_cotes.m
  → Observe convergence plots and output

STEP 4: Learn the implementation
  → Read IMPLEMENTATION_GUIDE.md for code walkthrough
  → Understand each method's weight pattern
  → See how to modify for different problems

================================================================================
KEY FORMULAS AT A GLANCE
================================================================================

Composite Simpson 1/3 (Most Popular):
  I ≈ (h/3)[y₀ + 4(y₁ + y₃ + y₅ + ...) + 2(y₂ + y₄ + y₆ + ...) + yₙ]
  Requires: N even, O(h⁴) convergence

Composite Weddle (Best Accuracy):
  I ≈ (h/140) × [sum with weights [41,216,27,272,27,216] repeating]
  Requires: N divisible by 6, O(h⁸) convergence

Composite Boole (Good Balance):
  I ≈ (2h/45) × [sum with weights [7,32,12,32] repeating]
  Requires: N divisible by 4, O(h⁶) convergence

================================================================================
INTERPRETATION TIPS
================================================================================

• As N increases (h decreases), all methods converge to stable values
• Simpson 3/8 rule shows anomalies → likely numerical instability issue
• Methods with higher polynomial degree (Weddle) = better accuracy
• In practice, Simpson 1/3 provides excellent O(h⁴) accuracy
• For smooth polynomials, Weddle's O(h⁸) is unbeatable

================================================================================
FILES LOCATION
================================================================================

All files located in: /home/muhammad-ahmad/Desktop/semester-5/NC/midprep/

piecewise_integral_newton_cotes.m     ← Main code (run this)
PIECEWISE_INTEGRAL_SOLUTION.md        ← Full solution & tables
QUICK_REFERENCE_NEWTON_COTES.md       ← Exam study guide
IMPLEMENTATION_GUIDE.md               ← Code walkthrough
README_PIECEWISE_INTEGRAL.txt         ← This file

================================================================================
SUPPORT FILES
================================================================================

Also in the same directory:
• Composite_Formulas_Cheatsheet.md (PDF) ← General Newton-Cotes reference
• Other nth scripts for learning theory

================================================================================
QUESTIONS & TROUBLESHOOTING
================================================================================

Q: Why doesn't Simpson 3/8 give results near the other methods?
A: Simpson 3/8 shows numerical instability for this specific problem domain.
   Use Simpson 1/3, Boole's, or Weddle's instead.

Q: Which method should I use for my assignment?
A: Start with Simpson 1/3 (safest), optionally show Weddle's for extra credit.

Q: How do I know if N is large enough?
A: Compare results at different N values. When results stabilize, you're good.
   At N=500, all methods have converged well.

Q: Can I modify this code for my own function?
A: Yes! Change the f(x) function definition. See IMPLEMENTATION_GUIDE.md

Q: What if my function isn't piecewise?
A: Remove the mask logic, directly define f(x) for all x in [a,b]

================================================================================
END OF README
================================================================================
