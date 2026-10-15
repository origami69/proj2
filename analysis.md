# PA2 analysis
Group 5: Arturo Becerra, Dylan Ward, Jonathan Brion, Damien Maristela, Xavier Guerrero
Anyone outside your group you discussed this with: nobody
AI assistance (tool, and what you used it for; write "none" if none): 
Google Gemini; to explain errors and how existing code works (like what all the argument slots 
of a function mean)

---

## Part C — can the kernel see your threads?

**System 1**
- Machine and OS: WSL2:
    - uname 
        Linux DESKTOP-J2D4250 6.18.54.1-microsoft-standard-WSL2 #1 SMP PREEMPT_DYNAMIC Tue Oct  6 19:13:07 UTC 2026 x86_64 x86_64 x86_64 GNU/Linux
    - sudo lshw -short
                            system       Computer
        /0                     bus        Motherboard
        /0/1                   memory     16GiB System memory
        /0/2                   processor  12th Gen Intel(R) Core(TM) i5-12400F
    - lsb_release -a
        Distributor ID: Ubuntu
        Description:    Ubuntu 24.04.4 LTS
        Release:        24.04
        Codename:       noble
    - gcc --version
        gcc (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0
- Command used to count threads:
    ls /proc/$(pgrep -n tcount)/task | wc -l
- Threads before the pthread_create calls:
    1
- Threads after:
    5
- Difference:
    4
- Does the difference match the number you asked for?
    Yes indeed I ran ./tcount --threads 4 
-**System 2** (optional but recommended — the course server and your laptop)
    I DONT WANNA DO THE SCHOOL SERVER SINCE I DONT WANNA GET THE VPN
- Machine and OS:
- Command used:
- Before / after / difference:

**One-to-one or many-to-one?** Two sentences. Say what you would have seen
instead if the mapping were many-to-one.

---

## Part D — measurements

Machine used for the timings:  WSL2:
    - uname -a
        Linux DESKTOP-J2D4250 6.18.54.1-microsoft-standard-WSL2 #1 SMP PREEMPT_DYNAMIC Tue Oct  6 19:13:07 UTC 2026 x86_64 x86_64 x86_64 GNU/Linux
    - sudo lshw -short
                               system     Computer
        /0                     bus        Motherboard
        /0/1                   memory     16GiB System memory
        /0/2                   processor  12th Gen Intel(R) Core(TM) i5-12400F
    - lsb_release -a
        Distributor ID: Ubuntu
        Description:    Ubuntu 24.04.4 LTS
        Release:        24.04
        Codename:       noble
    - gcc --version
        gcc: (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0
Core count, and the command you got it from:
nproc 
12
Plugged in, or on battery:
PLUGGED IN DESKTOP
### Speedup table

`--n 20000000 --reps 3`. Speedup is against your own 1-thread run.

| threads | fill_s | sum_s | wall_s | sum-phase speedup | whole-program speedup |
|---|---|---|---|---|---|
| 1 |0.1546 |0.5874 |0.7420 |1.00 |1.00 |
| 2 |0.1506 |0.3303 |0.4809 |1.77838328792 |1.54294032023|
| 4 |0.1544 |0.2029 |0.3573 |2.89502217841 |2.07668625805 |
| 8 |0.1513 |0.1291 |0.2804 |4.54996127033 |2.64621968616 |
| 12 (cores) |0.1539 |0.0968 |0.2508 |6.06818181818 |2.95853269537 |

### The serial fraction

- S = fill_s / wall_s at one thread = 0.1546/0.7420 = 0.208355795148
- Amdahl ceiling_1 = 1 / S = 4.79948253558
- Amdahl ceiling_2 = 1 / (S + (1-S)/N) = 3.64529599607 (N = 12 here for my system)
### The four questions

**1. Your measured S, and the ceiling it implies.**
   -The S is the portion that is serial which means about 0.79 or 79% is parallizable (1 - 0.208355795148 is roughly .79)
    amdahls ceiling gives us the maximum possible speedup we can achieve
**2. What whole-program speedup did you actually reach at the core count? How
far below the ceiling, and one reason for the gap.**
    -The total speedup for program is roughly 2.95853269537 which is nowheree near the ceiling of 4.79948253558 or 3.64529599607
    the difference between the ceiling and tested speedup is:
    4.79948253558 - 2.95853269537 = 1.84094984021 (62.225% difference)
    3.64529599607 - 2.95853269537 = 0.6867633007  (23.213% difference)  
    The original equation you told us to use of 1/S assumes that the number of cores approaches inifnity but that is simply not true in this case we are nowhere near a large enough core count to make that assumption
    Then I made a seperate field for the true equation at the core count and its more like a 23.2% difference and one reason I can think of is that because threading incurs a certain amount of overhead since we have to create each thread, and destroy each thread, then when each thread is psuhed into the scheduler and that means we need to switch context between each thread and other processes or threads which is another overhead asccociated with joining.

**3. Why is the sum-phase speedup so much better than the whole-program
speedup? Which one would a user of this program notice?**
    This is because the sum phase is optimized using multithreading and we get smaller times when we approach the core count amount of threads but the whole prgram is not just the optimized sums but also includes some seriable portions of the code like the fill_s phase which will always be near the same times with little variation and it also includes the optimized sums phase. 
    The user would notice the whole program speed up since they are the ones waiting for a program to finish and not waiting for some sums to finish.
**4. Why does `total` change in its last digits when the thread count changes,
and why is that not a bug?**
    This is a property of floating point numbers since we use a fixed amount of bits usually 32 bits to represent which has
    1  bit for the sign 8 bits for the exponent and 23 bits for the mantissa and can lead  rounding off some bits so we can align certain numbers correctly.
