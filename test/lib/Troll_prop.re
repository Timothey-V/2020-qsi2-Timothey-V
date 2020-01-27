open Framework;
open QCheckRely;
open Generator.Fantasy;
open Lib.Troll;

let {describe} = extendDescribe(QCheckRely.Matchers.matchers);

describe("Troll Invariance", ({test}) => {
  test("Troll score should be 0 when all elves resurrected", ({expect}) => {
    QCheck.Test.make(
      ~count=1000,
      ~name="Troll score should be 0 when all elves resurrected",
      troll_arbitrary,
      troll =>
      all_elves_resurrected(troll) |> scoring == 0
    )
    |> expect.ext.qCheckTest;
    ();
  });
  test("Troll score should always be >= 0", ({expect}) => {
    QCheck.Test.make(
      ~count=1000,
      ~name="Troll score should always be >= 0",
      troll_arbitrary,
      troll =>
      scoring(troll) >= 0
    )
    |> expect.ext.qCheckTest;
    ();
  });
});

describe("Troll Inverse", ({test}) => {
  test("oops_he_survived should always be inverse of i_got_one", ({expect}) => {
    QCheck.Test.make(
      ~count=1000,
      ~name="oops_he_survived should always be inverse of i_got_one",
      troll_elf_arbitrary,
      ((troll,elf)) =>
      scoring(troll |> i_got_one(elf) |> oops_he_survived(elf)) == scoring(troll)
    )
    |> expect.ext.qCheckTest;
    ();
  })
});

describe("Troll Analogy", ({test}) => {
  test("i_got_one and i_got should be consistent", ({expect}) => {
    QCheck.Test.make(
      ~count=1000,
      ~name="i_got_one and i_got should be consistent",
      troll_elf_int_arbitrary,
      ((troll,elf,num)) => {
      let trollRef = ref (troll);
      for (x in 1 to num) {
        trollRef := i_got_one(elf,trollRef^);
      }
      scoring(trollRef^) == scoring (troll |> i_got(num,elf))
      })
    |> expect.ext.qCheckTest;
    ();
  })
});

describe("Troll Idempotence", ({test}) => {
  test(
    "all_elves_of_a_kind_resurrected brings the Troll killing list to a stable state",
    ({expect}) => {
      QCheck.Test.make(
        ~count=1000,
        ~name="all_elves_of_a_kind_resurrected brings the Troll killing list to a stable state",
        troll_elf_int_arbitrary,
        ((troll,elf,num)) => {
        let num = num +1 ;
        let trollRef = ref (troll);
        for (x in 1 to num) {
          trollRef := all_elves_of_a_kind_resurrected(elf,trollRef^);
        } ;  
        scoring (trollRef^) == scoring(troll |> all_elves_of_a_kind_resurrected(elf))
        })
        |> expect.ext.qCheckTest;
      ();
  })
});

describe("Troll Metamorphism", ({test}) => {
  test(
    "i got one increase correctly the score",
    ({expect}) => {
      QCheck.Test.make(
        ~count=1000,
        ~name="i got one increase correctly the score",
        troll_elf_arbitrary,
        ((troll, elf)) => 
             Lib.Elf.value(elf) + scoring(troll) == scoring(i_got_one(elf, troll))
          
      )
      |> expect.ext.qCheckTest;      ()
    })
});

describe("Troll Injection", ({test}) => {
  test(
    "2 different entrances that do not lead to the same exit",
    ({expect}) => {
      QCheck.Test.make(
        ~count=1000,
        ~name="2 different entrances that do not lead to the same exit",
        troll_two_elves_arbitrary,
        ((troll, elfOne, elfTwo)) => {
            elfOne != elfTwo ?
            i_got_one(elfOne, troll) != i_got_one(elfTwo, troll) : true;
        }
          
      )
      |> expect.ext.qCheckTest;      ()
  })
});

