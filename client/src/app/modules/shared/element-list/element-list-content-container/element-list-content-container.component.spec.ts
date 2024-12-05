import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ElementListContentContainerComponent } from './element-list-content-container.component';

describe('ElementListContentContainerComponent', () => {
  let component: ElementListContentContainerComponent;
  let fixture: ComponentFixture<ElementListContentContainerComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [ElementListContentContainerComponent]
    })
    .compileComponents();

    fixture = TestBed.createComponent(ElementListContentContainerComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
